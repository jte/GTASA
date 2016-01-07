#pragma once

//tBikeHandlingData
//tHandlingData

class cHandlingDataMgr
{
public:
	/* ConvertBikeDataToGameUnits(tBikeHandlingData*)
	* ConvertBikeDataToWorldUnits(tBikeHandlingData*)
	* ConvertDataToGameUnits(tHandlingData*)
	* ConvertDataToWorldUnits(tHandlingData*)
	*/ 
	uint32_t FindExactWord(char*, char*, int, int);
	/* GetBoatPointer(unsigned char)
	* GetFlyingPointer(unsigned char)
	* GetHandlingId(char const*)
	* Initialise()
	* LoadHandlingData()
	* cHandlingDataMgr()
	*/
private:
};