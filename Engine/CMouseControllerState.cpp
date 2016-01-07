#include "StdInc.h"

bool CMouseControllerState::CheckForInput()
{
    return lmb || rmb || mmb || wheelUp || wheelDown || bmx1 || bmx2 || X != 0.0f || Y != 0.0f;
}

void CMouseControllerState::Clear()
{
    lmb = 0;
    rmb = 0;
    mmb = 0;
    wheelUp = 0;
    wheelDown = 0;
    bmx1 = 0;
    bmx2 = 0;
}

CMouseControllerState::CMouseControllerState()
{
    lmb = 0;
    rmb = 0;
    mmb = 0;
    wheelUp = 0;
    wheelDown = 0;
    Z = 0.0f;
    X = 0.0f;
    Y = 0.0f;
    bmx1 = 0;
    bmx2 = 0;
}