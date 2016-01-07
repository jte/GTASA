#pragma once

class CMessages
{
public:
    /* AddBigMessage(unsigned short*, unsigned int, unsigned short)
	* AddBigMessageQ(unsigned short*, unsigned int, unsigned short)
	* AddBigMessageWithNumber(unsigned short*, unsigned int, unsigned short, int, int, int, int, int, int)
	* AddBigMessageWithNumberQ(unsigned short*, unsigned int, unsigned short, int, int, int, int, int, int)
	* AddMessage(char const*, unsigned short*, unsigned int, unsigned short, bool)
	* AddMessageJumpQ(char const*, unsigned short*, unsigned int, unsigned short, bool)
	* AddMessageJumpQWithNumber(unsigned short*, unsigned int, unsigned short, int, int, int, int, int, int, bool)
	* AddMessageJumpQWithString(unsigned short*, unsigned int, unsigned short, unsigned short*, bool)
	* AddMessageWithNumber(char const*, unsigned short*, unsigned int, unsigned short, int, int, int, int, int, int, bool)
	* AddMessageWithString(unsigned short*, unsigned int, unsigned short, unsigned short*, bool)
	* AddToPreviousBriefArray(char const*, unsigned short*, int, int, int, int, int, int, unsigned short*)
	* ClearAllMessagesDisplayedByGame(unsigned char)
	* ClearMessages(unsigned char)
	* ClearPreviousBriefArray()
	* ClearSmallMessagesOnly()
	* ClearThisBigPrint(unsigned short*)
	* ClearThisBigPrintNow(unsigned int)
	* ClearThisPrint(unsigned short*)
	* CutString(short, char*, char**)
	* Display(unsigned char)
	* 
	* GxtStringCompare(unsigned short*, unsigned short*, unsigned short)
	*/ 
	static size_t GetGxtStringLength(unsigned short* string);
    static void GxtStringCopy(unsigned short* destination, unsigned short* source, unsigned short length);
	/* Init()
	* InsertNumberInString(unsigned short*, int, int, int, int, int, int, unsigned short*)
	* InsertPlayerControlKeysInString(unsigned short*)
	* InsertStringInString(unsigned short*, unsigned short*)
	* Process()
	* ReloadPreviousBriefArray()*/
private:
    /* BIGMessages
	* BriefMessages
	* PreviousBriefs*/
};