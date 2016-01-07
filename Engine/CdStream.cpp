#include "StdInc.h"

struct Queue
{
    uint32_t *queue;
    uint32_t head;
    uint32_t tail;
    uint32_t size;
};

struct CdStream
{ //sizeof=48bytes
    uint32_t offset;
    uint32_t size;
    void *buffer;
    uint8_t pad1;
    bool semaphore_guard;
    bool processing;
    uint8_t pad2;
    uint32_t error_code;
    HANDLE semaphore;
    HANDLE stream;
    OVERLAPPED overlapped;
};


static char gCdImageNames[32][64];
static HANDLE gCdImageHandles[32];
static bool gUseOverlappedIO;
static uint32_t gStreamCreateFlags;
static bool gStreamingInitialized;
static uint32_t gStreamCount;
static CdStream *gStreams;
static Queue gStreamQueue;
static HANDLE gStreamSemaphore;
static HANDLE gStreamThread;
static uint32_t gOpenStreamCount;
static uint32_t gStreamLastPosn;

size_t CdStreamGetLastPosn()
{
    return gStreamLastPosn;
}

void CdStreamInit(size_t stream_count)
{
    memset(gCdImageHandles, 0, sizeof(gCdImageHandles));
    for (size_t i = 0; i < 32; i++)
    {
        gCdImageNames[i][0] = '\0';
    }
    unsigned long dummy;
    unsigned long bytes_per_sector;
    GetDiskFreeSpaceA(NULL, &dummy, &bytes_per_sector, &dummy, &dummy);
    gUseOverlappedIO = true;
    gStreamCreateFlags = (((bytes_per_sector <= 2048) ? FILE_FLAG_NO_BUFFERING : 0)) | FILE_FLAG_OVERLAPPED;
    gStreamingInitialized = false;
    void *buffer = CMemoryMgr::MallocAlign(2048, bytes_per_sector);
    SetLastError(0);
    gOpenStreamCount = 0;
    gStreamCount = stream_count;
    gStreams = (CdStream*)LocalAlloc(0x40u, sizeof(CdStream) * stream_count);
    CdStreamOpen("MODELS\\GTA3.IMG");
    bool success = CdStreamRead(0, buffer, 0, 1);
    CdStreamRemoveImages();
    gStreamingInitialized = true;
    if (!success)
    {
        gUseOverlappedIO = false;
        gStreamCreateFlags &= ~FILE_FLAG_OVERLAPPED;
    }
    CdStreamInitThread();
    CMemoryMgr::FreeAlign(&buffer);
}

int CdStreamOpen(const char *filename)
{
    size_t i;
    for (i = 0; i < 32; i++)
    {
        if (!gCdImageHandles[i])
        {
            break;
        }
    }
    SetLastError(0);
    gCdImageHandles[i] = CreateFileA(filename, GENERIC_READ, 1, NULL, OPEN_EXISTING, gStreamCreateFlags | FILE_FLAG_RANDOM_ACCESS | FILE_ATTRIBUTE_READONLY, NULL);
    if (gCdImageHandles[i] == INVALID_HANDLE_VALUE )
    {
        return false;
    }
    strcpy(gCdImageNames[i], filename);
    return i << 24;
}

DWORD CALLBACK CdStreamThread(void*)
{
    while (true)
    {
        WaitForSingleObject(gStreamSemaphore, INFINITE);
        CdStream *stream;
        if (gStreamQueue.head == gStreamQueue.tail)
        {
            stream = &gStreams[-1];
        }
        else
        {
            stream = &gStreams[gStreamQueue.queue[gStreamQueue.head]];
        }
        stream->processing = true;
        if (!stream->error_code)
        {
            if (gUseOverlappedIO)
            {
                stream->overlapped.Offset = stream->offset * 0x800;
                if (ReadFile(stream->stream, stream->buffer, stream->size * 0x800, 0, &stream->overlapped) == TRUE)
                {
                    stream->error_code = 0;
                    goto LABEL_13;
                }
                if (GetLastError() != ERROR_IO_PENDING)
                {
                    stream->error_code = 254;
                    goto LABEL_13;
                }
                unsigned long dummy;
                stream->error_code = GetOverlappedResult(stream->stream, &stream->overlapped, &dummy, 1) != TRUE ? 254 : 0;
            }
            else
            {
                SetFilePointer(stream->stream, stream->offset * 0x800, 0, 0);
                unsigned long dummy;
                stream->error_code = ReadFile(stream->stream, stream->buffer, stream->size * 0x800, &dummy, 0) != FALSE ? 0 : 254;
            }
        }
LABEL_13:
        if (gStreamQueue.head != gStreamQueue.tail)
        {
            gStreamQueue.head = (gStreamQueue.head + 1) % gStreamQueue.size;
        }
        stream->size = 0;
        if (stream->semaphore_guard)
        {
            ReleaseSemaphore(stream->semaphore, 1, 0);
        }
        stream->processing = 0;
    }
    return 0; // should be last stream's error code
}

void CdStreamInitThread()
{
    SetLastError(0);
    for (size_t i = 0; i < gStreamCount; i++)
    {
        gStreams[i].semaphore = CreateSemaphoreA(NULL, 0, 2, NULL);
        if (!gStreams[i].semaphore)
        {
            printf("%s: failed to create sync semaphore\n", "cdvd_stream");
        }
    }
    gStreamQueue.queue = (uint32_t*)LocalAlloc(0x40, 4 * gStreamCount + 4);
    gStreamQueue.head = 0;
    gStreamQueue.tail = 0;
    gStreamQueue.size = gStreamCount + 1;
    gStreamSemaphore = CreateSemaphoreA(0, 0, 5, "CdStream");
    if (!gStreamSemaphore)
    {
        printf("%s: failed to create stream semaphore\n", "cdvd_stream");
    }
    unsigned long tid;
    gStreamThread = CreateThread(0, 0x10000u, CdStreamThread, 0, 4u, &tid);
    if (!gStreamThread)
    {
        printf("%s: failed to create streaming thread\n", "cdvd_stream");
    }
    SetThreadPriority(gStreamThread,  GetThreadPriority(GetCurrentThread()));
    ResumeThread(gStreamThread);
}

bool CdStreamRead(uint32_t stream_id, void *buffer, uint32_t stream_ptr, uint32_t sector_count)
{
    CdStream *stream = &gStreams[stream_id];
    gStreamLastPosn = stream_ptr + sector_count;
    stream->stream = gCdImageHandles[stream_ptr >> 24];
    SetLastError(0);
    if (!gStreamingInitialized)
    {
        return false;
    }
    if (!stream->size && !stream->processing)
    {
        stream->error_code = 0;
        stream->offset = stream_ptr & 0xFFFFFF;
        stream->size = sector_count;
        stream->buffer = buffer;
        stream->semaphore_guard = false;
        gStreamQueue.tail = stream_id;
        gStreamQueue.tail = (gStreamQueue.tail + 1) % gStreamQueue.size;
        if (!ReleaseSemaphore(gStreamSemaphore, 1, 0))
        {
            printf("Signal Sema Error\n");
        }
        return true;
    }
    if (gUseOverlappedIO)
    {
        stream[stream_id].overlapped.Offset = (stream_ptr & 0xFFFFFF) * 0x800;
        return ReadFile(stream->stream, buffer, sector_count * 0x800, 0, &stream[stream_id].overlapped) || GetLastError() == ERROR_IO_PENDING;
    }
    SetFilePointer(stream->stream, (stream_ptr & 0xFFFFFF) * 0x800, 0, 0);
    unsigned long dummy;
    return ReadFile(stream->stream, buffer, sector_count * 0x800, &dummy, 0);
}

uint32_t CdStreamGetStatus(uint32_t stream_id)
{
    CdStream *stream = &gStreams[stream_id];
    if (gStreamingInitialized)
    {
        if (stream->processing)
        {
            return 255;
        }
        else if (stream->size)
        {
            return 250;
        }
        else if (stream->error_code != 0)
        {
            uint32_t t = stream->error_code;
            stream->error_code = 0;
            return t;
        }
    }
    else if (gUseOverlappedIO)
    {
        return WaitForSingleObjectEx(stream->stream, 0, TRUE) != 0 ? 255 : 0;
    }
    return 0;
}

void CdStreamRemoveImages()
{
    for (size_t i = 0; i < gStreamCount; i++)
    {
        CdStreamSync(i);
    }

    for (size_t i = 0; i < gOpenStreamCount; i++)
    {
        SetLastError(0);
        if (gCdImageHandles[i])
        {
            CloseHandle(gCdImageHandles[i]);
        }
        gCdImageHandles[i] = NULL;
        gCdImageNames[i][0] = '\0';
    }
    gOpenStreamCount = 0;
}

void CdStreamSync(uint32_t stream_id)
{
    CdStream *stream = &gStreams[stream_id];
    if (gStreamingInitialized)
    {
        if (stream->size)
        {
            stream->semaphore_guard = true;
            WaitForSingleObject(stream->semaphore, INFINITE);
        }
        stream->processing = false;
    }
    else if (gUseOverlappedIO && stream->stream)
    {
        unsigned long dummy;
        GetOverlappedResult(stream->stream, &stream->overlapped, &dummy, 1);
    }
}

void CdStreamShutdown()
{
    if (gStreamingInitialized)
    {
        LocalFree(gStreamQueue.queue);
        CloseHandle(gStreamSemaphore);
        CloseHandle(gStreamThread);
        for (size_t i = 0; i < gStreamCount; i++)
        {
            CloseHandle(gStreams[i].semaphore);
        }
    }
    LocalFree(gStreams);
}