#include "StdInc.h"

void CPed::Initialise()
{
    CPedType::Initialise();
    CCarEnterExit::SetAnimOffsetForEnterExitVehicle();
}

void CPed::SetMoveState(eMoveState moveState)
{
    m_moveState = moveState;
}