#pragma once

class CAEAudioUtility
{
public:
    /*
	* AudioLog10(float)
	* ConvertFromBytesToMS(unsigned int, unsigned int, unsigned short)
	* ConvertFromMSToBytes(unsigned int, unsigned int, unsigned short)
	* FindVehicleOfPlayer()
	* GetBankAndSoundFromScriptSlotAudioEvent(int*, int*, int*, int)
	* GetCurrentTimeInMilliseconds()
	* GetPiecewiseLinear(float, short, float (*) [2])
	* ResolveProbability(float)
	* StaticInitialise()
    */
    static float GetRandomNumberInRange(float start, float end);
    static int GetRandomNumberInRange(int start, int end);
private:
    //m_sfLogLookup
};