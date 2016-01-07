#pragma once

class CTimer
{
public:
	static void EndUserPause();
	static size_t GetCurrentTimeInCycles();
	static size_t GetCyclesPerFrame();
	static size_t GetCyclesPerMillisecond();
	static bool GetIsSlowMotionActive();
	static void Initialise();
	static void Resume();
	static void Shutdown();
	static void StartUserPause();
	static void Stop();
	static void Suspend();
	static void Update();
	static void UpdateVariables(float dt);
    static float GetTimeStep();
    static size_t GetFrameCounter();
    //
    static size_t GetCurrentTimeMs();
    static size_t GetCodePause();
    static size_t GetUserPause();
private:
    static bool bSkipProcessThisFrame;
	static bool bSlowMotionActive;
	static size_t game_FPS;
	static size_t m_CodePause;
	static size_t m_FrameCounter;
	static size_t m_UserPause;
    // Note: spelling error "MILLISECONDS" by R*
	static size_t m_snPPPPreviousTimeInMilliseconds;
	static size_t m_snPPPreviousTimeInMilliseconds;
	static size_t m_snPPreviousTimeInMilliseconds;
	static size_t m_snPreviousTimeInMilliseconds;
	static size_t m_snPreviousTimeInMillisecondsNonClipped;
	static size_t m_snTimeInMilliseconds;
	static size_t m_snTimeInMillisecondsNonClipped;
	static size_t m_snTimeInMillisecondsPauseMode;
	static float ms_fOldTimeStep;
	static float ms_fSlowMotionScale;
	static float ms_fTimeScale;
	static float ms_fTimeStep;
	static float ms_fTimeStepNonClipped;
};