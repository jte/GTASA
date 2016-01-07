#include "StdInc.h"

typedef uint64_t (*TimerCallback)(void);
static uint64_t renderStartTime;
static uint64_t renderTimerPauseTime;
static TimerCallback timerDef;
static uint32_t timerCyclesPerMillisecond;
static uint32_t renderTimerPauseCount;
static uint32_t timerRunning; // actually "timerStarted"

static uint64_t Win32GetTime()
{
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    return ((time.QuadPart << 32) & 0xffffffff00000000LL) | ((time.HighPart << 16) & 0xffff0000) | (time.LowPart & 0xffff);
}

static uint64_t RwGetTime()
{
    return RsTimer();
}

void CTimer::Stop()
{
    m_snPPPPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
    m_snPPPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
    m_snPPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
    m_snPreviousTimeInMilliseconds = m_snTimeInMilliseconds;
    timerRunning = 0;
    m_snTimeInMillisecondsNonClipped = m_snPreviousTimeInMillisecondsNonClipped;
}

void CTimer::Initialise()
{
    m_UserPause = 0;
    m_CodePause = 0;
    bSlowMotionActive = false;
    bSkipProcessThisFrame = false;
    m_snPPPPreviousTimeInMilliseconds = 0;
    m_snPPPreviousTimeInMilliseconds = 0;
    m_snPPreviousTimeInMilliseconds = 0;
    m_snPreviousTimeInMilliseconds = 0;
    m_snTimeInMillisecondsNonClipped = 0;
    m_snTimeInMilliseconds = 0;
    m_FrameCounter = 0;
    renderTimerPauseCount = 0;
    timerRunning = 0;
    game_FPS = 0;
    m_snPreviousTimeInMillisecondsNonClipped = 1;
    ms_fTimeScale = 1.0f;
    ms_fTimeStep = 1.0f;
    ms_fTimeStepNonClipped = 1.0f;
    LARGE_INTEGER Freq;
    if (QueryPerformanceFrequency(&Freq))
    {
        timerDef = Win32GetTime;
        timerCyclesPerMillisecond = Freq.LowPart / 1000;
    }
    else
    {
        timerDef = RwGetTime;
        timerCyclesPerMillisecond = 1;
    }
    renderStartTime = timerDef();
}

void CTimer::Shutdown()
{
    timerRunning = 0;
}

void CTimer::UpdateVariables(float dt)
{
    float dts = dt / timerCyclesPerMillisecond;
    m_snPreviousTimeInMillisecondsNonClipped += static_cast<size_t>(dts);
    ms_fOldTimeStep = dts / 20;
    dts = ClampMax(dts, 300);
    // clip to 300 ms
    if(dts > 300)
    {
        dts = 300;
    }
    m_snTimeInMilliseconds = static_cast<size_t>(dts);
    if (ms_fOldTimeStep < 1.0f/100.0f)
    {
        if (!m_UserPause && !m_CodePause && !CSpecialFX::bSnapShotActive)
        {
            ms_fOldTimeStep = 1.0f/100.0f;
        }
    }
    ms_fTimeStepNonClipped = ms_fTimeStep;
    if (ms_fOldTimeStep >= 3.0f)
    {
        ms_fTimeStep = 3.0f;
    }
    else if (ms_fOldTimeStep > 1.0f/100000.0f)
    {
        ms_fTimeStep = ms_fOldTimeStep;
    }
    else
    {
        ms_fTimeStep = 1.0f / 100000.0f;
    }
}

void CTimer::Suspend()
{
    if (timerRunning)
    {
        renderTimerPauseCount++;
        if(renderTimerPauseCount > 1)
        {
            renderTimerPauseTime = timerDef();
        }
    }
}

void CTimer::Resume()
{
    if (timerRunning)
    {
        if (renderTimerPauseCount == 1)
        {
            uint64_t current_time = timerDef();
            renderStartTime = current_time - renderTimerPauseTime;
        }
        renderTimerPauseCount--;
    }
}

size_t CTimer::GetCyclesPerMillisecond()
{
    return timerCyclesPerMillisecond;
}

size_t CTimer::GetCyclesPerFrame()
{
    // 20 ms/frame
    return GetCyclesPerMillisecond() * 20;
}

size_t CTimer::GetCurrentTimeInCycles()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.LowPart - static_cast<size_t>(renderStartTime);
}

bool CTimer::GetIsSlowMotionActive()
{
    return ms_fTimeScale < 1.0f;
}

void CTimer::StartUserPause()
{
    m_UserPause = 1;
}

void CTimer::EndUserPause()
{
    m_UserPause = 0;
}

float CTimer::GetTimeStep()
{
    return ms_fTimeStep;
}

size_t CTimer::GetCurrentTimeMs()
{
    return m_snTimeInMilliseconds;
}

size_t CTimer::GetCodePause()
{
    return m_CodePause;
}

size_t CTimer::GetUserPause()
{
    return m_UserPause;
}