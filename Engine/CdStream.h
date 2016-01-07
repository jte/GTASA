#pragma once

//* CdStreamClose(int)
size_t CdStreamGetLastPosn();
uint32_t CdStreamGetStatus(uint32_t stream_id);
void CdStreamInit(size_t stream_count);
void CdStreamInitThread();
int CdStreamOpen(const char *filename);
bool CdStreamRead(uint32_t stream_id, void *buffer, uint32_t stream_ptr, uint32_t sector_count);
void CdStreamRemoveImages();
//* CdStreamResume()
void CdStreamShutdown();
//* CdStreamSuspend()
void CdStreamSync(uint32_t stream_id);
DWORD CALLBACK CdStreamThread(void*);