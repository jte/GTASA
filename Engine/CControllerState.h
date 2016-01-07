#pragma once

class CControllerState
{
public:	
    bool CheckForInput();
	void Clear();
public:
    signed short        LeftStickX; // move/steer left (-128?)/right (+128)
    signed short        LeftStickY; // move back(+128)/forwards(-128?)
    signed short        RightStickX; // numpad 6(+128)/numpad 4(-128?)
    signed short        RightStickY;
    
    signed short        LeftShoulder1;
    signed short        LeftShoulder2;
    signed short        RightShoulder1; // target / hand brake
    signed short        RightShoulder2; 
    
    signed short        DPadUp; // radio change up
    signed short        DPadDown; // radio change down
    signed short        DPadLeft;
    signed short        DPadRight;

    signed short        Start;
    signed short        Select;

    signed short        ButtonSquare; // jump / reverse
    signed short        ButtonTriangle; // get in/out
    signed short        ButtonCross; // sprint / accelerate
    signed short        ButtonCircle; // fire

    signed short        ShockButtonL;
    signed short        ShockButtonR; // look behind

    signed short        m_bChatIndicated;
    signed short        m_bPedWalk;
    signed short        m_bVehicleMouseLook;
    signed short        m_bRadioTrackSkip;
};