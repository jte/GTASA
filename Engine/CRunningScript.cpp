#include "StdInc.h"

void CRunningScript::ProcessCommands2100To2199(int cmd)
{
    switch(cmd)
    {
        case DO_CAMERA_BUMP:
            CollectParameters(2);
            TheCamera.GetActiveCam().DoCamBump(ScriptParams[0], ScriptParams[1]);
        return 0;
        case GET_CURRENT_DATE:
            ScriptParams[0] = CClock::ms_nGameClockDays;
            ScriptParams[1] = CClock::ms_nGameClockMonths;
            StoreParameters(2);
        return 0;
        case SET_OBJECT_ANIM_SPEED:
    }
}