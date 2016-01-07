#include "StdInc.h"

int16_t CPad::GetSteeringLeftRight()
{
	if (DisablePlayerControls)
	{
		return 0;
	}
	switch (Mode)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			SteeringLeftRightBuffer[0] = NewState.LeftStickX;
			return SteeringLeftRightBuffer[DrunkInputDelay];
		default:
			return 0;
	}
}

int16_t CPad::GetSteeringUpDown()
{
	if (DisablePlayerControls)
	{
		return 0;
	}
	switch ( this->Mode )
	{
		case 0:
		case 1:
		case 2:
		case 3:
			return NewState.LeftStickY;
		default:
			return 0;
	}
}

int16_t CPad::GetPedWalkUpDown()
{
	if (DisablePlayerControls)
	{
		return 0;
	}
    switch (Mode)
    {
		case 0:
		case 1:
		case 2:
		case 3:
			return NewState.LeftStickX;
		default:
			return 0;
    }
}

int16_t CPad::GetPedWalkLeftRight()
{
	if (DisablePlayerControls)
	{
		return 0;
	}
    switch (Mode)
    {
		case 0:
		case 1:
		case 2:
		case 3:
			return NewState.LeftStickY;
		default:
			return 0;
    }
}

bool CPad::GetLookLeft(bool)
{
	if (DisablePlayerControls)
	{
		return false;
	}
	if (NewState.LeftShoulder2 != 0 && !OldState.LeftShoulder2)
	{
		return false;
	}
	if (NewState.RightShoulder2 == 0 && OldState.RightShoulder2)
	{
		return false;
	}
	return NewState.LeftShoulder2 && !NewState.RightShoulder2;
}

bool CPad::GetLookRight(bool)
{
	if (DisablePlayerControls)
	{
		return false;
	}
	if (NewState.RightShoulder2 != 0 && !OldState.RightShoulder2)
	{
		return false;
	}
	if (NewState.LeftShoulder2 == 0 && OldState.LeftShoulder2)
	{
		return false;
	}
	return NewState.RightShoulder2 && !NewState.LeftShoulder2;
}

bool CPad::GetLookBehindForCar()
{
	if (DisablePlayerControls)
	{
		return false;
	}
	return (NewState.LeftShoulder2 || OldState.LeftShoulder2) &&
		   (NewState.RightShoulder2 || OldState.RightShoulder2);
}

bool CPad::GetLookBehindForPed()
{
	if (DisablePlayerControls)
	{
		return false;
	}
	return this->NewState.ShockButtonR != 0;
}

bool CPad::GetHorn(bool)
{
	if (DisablePlayerControls)
	{
		return false;
	}
	switch (Mode)
	{
		case 0:
		case 3:
			return NewState.ShockButtonL != 0;
		case 1:
			return NewState.LeftShoulder1 != 0;
		case 2:
			return NewState.RightShoulder1 != 0;
		default:
			return false;
	}
}

void CPad::SetDrunkInputDelay(int drunkInputDelay)
{
	DrunkInputDelay = drunkInputDelay;
}

bool CPad::GetJump(bool)
{
	if (DisablePlayerControls)
	{
		return false;
	}
	switch ( this->Mode )
	{
		case 0:
		case 3:
			return CycleWeaponRightJustDown();
		case 1:
			return ActionJustDown();
		case 2:
			return AimJustDown();
		default:
			return false;
	}
}

bool CPad::GetHydraulicJump()
{
	if (DisablePlayerControls)
	{
		return false;
	}
	return NewState.ShockButtonR != 0;
}

int16_t CPad::GetCarGunFired(bool, bool)
{
	if (this->DisablePlayerControls)
	{
		return 0;
	}
	switch (Mode)
	{
		case 0:
		case 1:
		case 2:
			if (NewState.ButtonCircle)
			{
				return 1;
			}
			if (!bDisablePlayerFireWeaponWithL1 && NewState.LeftShoulder1)
			{
				return 2;
			}
		break;
		case 3:
			if (NewState.RightShoulder1 )
			{
				return 1;
			}
		break;
		default:
			return 0;
	}
	return 0;
}

int16_t CPad::GetHandBrake()
{
	if (DisablePlayerControls)
	{
		return 0;
	}
	switch (Mode)
	{
		case 0:
		case 1:
			return NewState.RightShoulder1;
		case 2:
			return NewState.ButtonTriangle;
		case 3:
			return NewState.LeftShoulder1;
		break;
		default:
			return 0;
	}
}