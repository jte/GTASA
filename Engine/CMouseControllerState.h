#pragma once

class CMouseControllerState
{
public:	
    CMouseControllerState();
	bool CheckForInput();
	void Clear();
    //
private:
    uint8_t lmb;
    uint8_t rmb;
    uint8_t mmb;
    uint8_t wheelUp;
    uint8_t wheelDown;
    uint8_t bmx1;
    uint8_t bmx2;
    uint8_t __align;
    float Z;
    float X;
    float Y;
};
