#include "StdInc.h"

DIOBJECTDATAFORMAT DIMouseDataObjectFormat[] =
{
    {&GUID_XAxis, 0, DIDFT_AXIS|DIDFT_ANYINSTANCE, 0},
    {&GUID_YAxis, 4, DIDFT_AXIS|DIDFT_ANYINSTANCE, 0},
    {&GUID_ZAxis, 8, DIDFT_AXIS|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 12, DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0},
    {NULL, 13, DIDFT_BUTTON|DIDFT_ANYINSTANCE, 0},
    {NULL, 14, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 15, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 16, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 17, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 18, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0},
    {NULL, 19, DIDFT_BUTTON|DIDFT_ANYINSTANCE|DIDFT_OPTIONAL, 0}
};

DIDATAFORMAT DIMouseDataFormat = 
{
    sizeof(DIMouseDataFormat), 
    sizeof(DIOBJECTDATAFORMAT), 
    DIDF_RELAXIS, 
    20, 
    ELEMS_COUNT(DIMouseDataObjectFormat), 
    DIMouseDataObjectFormat
};

static void diMouseInit(bool exclusive)
{

    IDirectInput8A* input = PSGLOBAL(diInterface);
    HRESULT res = input->CreateDevice(GUID_SysMouse, &PSGLOBAL(diMouse), NULL);
    if (SUCCEEDED(res))
    {
        IDirectInputDevice8A* mouse = PSGLOBAL(diMouse);
        res = mouse->SetDataFormat(&DIMouseDataFormat);
        if (SUCCEEDED(res))
        {
            res = mouse->SetCooperativeLevel(PSGLOBAL(window), DISCL_FOREGROUND | (exclusive ? (DISCL_EXCLUSIVE) : DISCL_NONEXCLUSIVE));
            if (SUCCEEDED(res))
            {
                res = mouse->Acquire();
            }
        }
    }
}

CMouseControllerState* CMousePointerStateHelper::GetMouseSetUp()
{
    static CMouseControllerState mouseController;

    if (!PSGLOBAL(diMouse))
    {
        bool exclusive = false;
        if (!FrontEndMenuManager.bMenuActive && isCurrentModeFullscreen())
        {
            exclusive = true;
        }
        diMouseInit(exclusive);
    }
    if (!PSGLOBAL(diMouse))
    {
        IDirectInputDevice8A* mouse = PSGLOBAL(diMouse);
        DIDEVCAPS caps;
        caps.dwSize = sizeof(caps);
        mouse->GetCapabilities(&caps);
    
        switch (caps.dwDevType)
        {
        case DI8DEVTYPEMOUSE_FINGERSTICK:
        case DI8DEVTYPEMOUSE_TOUCHPAD:
        case DI8DEVTYPEMOUSE_TRACKBALL:
        case DI8DEVTYPEMOUSE_ABSOLUTE:
        case 7:
        case 8:
            mouseController.mmb = 1;
        case DI8DEVTYPEMOUSE_TRADITIONAL:
            mouseController.rmb = 1;
        case DI8DEVTYPEMOUSE_UNKNOWN:
            mouseController.lmb = 1;
        break;
        default:
        break;
        }
        if (caps.dwFlags == DIDC_ATTACHED | DIDC_POLLEDDEVICE)
        {
            mouseController.wheelUp = 1;
            mouseController.wheelDown = 1;
        }
    }
    return &mouseController;
}