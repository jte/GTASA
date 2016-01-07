#include "StdInc.h"

static CKeyboardState currKeyState;
static CKeyboardState prevKeyState;

void CControllerConfigManager::InitialiseControllerActionNameArray()
{
    uint16_t event[40];

    AsciiToGxtChar("PED_LOOKBEHIND", event);
    CMessages::GxtStringCopy(m_eventNames[LOOK_BEHIND], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_CYCLE_WEAPON_LEFT", event);
    CMessages::GxtStringCopy(m_eventNames[PREVIOUS_WEAPON], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_CYCLE_WEAPON_RIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[NEXT_WEAPON], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_LOCK_TARGET", event);
    CMessages::GxtStringCopy(m_eventNames[AIM_WEAPON], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_JUMPING", event);
    CMessages::GxtStringCopy(m_eventNames[JUMP], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_SPRINT", event);
    CMessages::GxtStringCopy(m_eventNames[SPRINT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("SNEAK_ABOUT", event);
    CMessages::GxtStringCopy(m_eventNames[WALK], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_CYCLE_TARGET_LEFT", event);
    CMessages::GxtStringCopy(m_eventNames[CYCLE_TARGET_LEFT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_CYCLE_TARGET_RIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[CYCLE_TARGET_RIGHT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_LOCK_TARGET", event);
    CMessages::GxtStringCopy(m_eventNames[AIM_WEAPON], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_CENTER_CAMERA_BEHIND_PLAYER", event);
    CMessages::GxtStringCopy(m_eventNames[CENTER_CAMERA_BEHIND_PLAYER], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_LOOKBEHIND", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_LOOK_BEHIND], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_DUCK", event);
    CMessages::GxtStringCopy(m_eventNames[CROUCH], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_ANSWER_PHONE", event);
    CMessages::GxtStringCopy(m_eventNames[ACTION], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_STEERLEFT", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_LEFT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_STEERRIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_RIGHT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_STEERUP", event);
    CMessages::GxtStringCopy(m_eventNames[STEER_FORWARDS_DOWN], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_STEERDOWN", event);
    CMessages::GxtStringCopy(m_eventNames[STEER_BACK_UP], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_LOOKLEFT", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_LOOK_LEFT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_LOOKRIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_LOOK_RIGHT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_MOUSELOOK", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_MOUSE_LOOK], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_HORN", event);
    CMessages::GxtStringCopy(m_eventNames[HORN], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_HANDBRAKE", event);
    CMessages::GxtStringCopy(m_eventNames[HANDBRAKE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_ACCELERATE", event);
    CMessages::GxtStringCopy(m_eventNames[ACCELERATE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_BRAKE", event);
    CMessages::GxtStringCopy(m_eventNames[BRAKE_REVERSE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_RADIO_STATION_UP", event);
    CMessages::GxtStringCopy(m_eventNames[RADIO_NEXT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_RADIO_STATION_DOWN", event);
    CMessages::GxtStringCopy(m_eventNames[RADIO_PREVIOUS], event, ELEMS_COUNT(event));

    AsciiToGxtChar("TOGGLE_SUBMISSIONS", event);
    CMessages::GxtStringCopy(m_eventNames[SUB_MISSION], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_SNIPER_ZOOM_IN", event);
    CMessages::GxtStringCopy(m_eventNames[ZOOM_IN], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_SNIPER_ZOOM_OUT", event);
    CMessages::GxtStringCopy(m_eventNames[ZOOM_OUT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_1RST_PERSON_LOOK_LEFT", event);
    CMessages::GxtStringCopy(m_eventNames[FIRST_PERSON_LOOK_LEFT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_1RST_PERSON_LOOK_RIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[FIRST_PERSON_LOOK_RIGHT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_1RST_PERSON_LOOK_UP", event);
    CMessages::GxtStringCopy(m_eventNames[FIRST_PERSON_LOOK_UP], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_1RST_PERSON_LOOK_DOWN", event);
    CMessages::GxtStringCopy(m_eventNames[FIRST_PERSON_LOOK_DOWN], event, ELEMS_COUNT(event));

    AsciiToGxtChar("SHOW_MOUSE_POINTER_TOGGLE", event);
    CMessages::GxtStringCopy(m_eventNames[TOGGLE_MOUSE_POINTER], event, ELEMS_COUNT(event));

    AsciiToGxtChar("CAMERA_CHANGE_VIEW_ALL_SITUATIONS", event);
    CMessages::GxtStringCopy(m_eventNames[CHANGE_CAMERA], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_FIREWEAPON", event);
    CMessages::GxtStringCopy(m_eventNames[FIRE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("PED_FIREWEAPON_ALT", event);
    CMessages::GxtStringCopy(m_eventNames[FIRE_ALT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_FIREWEAPON", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_FIRE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_FIREWEAPON_ALT", event);
    CMessages::GxtStringCopy(m_eventNames[VEHICLE_SECONDARY_FIRE], event, ELEMS_COUNT(event));

    AsciiToGxtChar("VEHICLE_ENTER_EXIT", event);
    CMessages::GxtStringCopy(m_eventNames[ENTER_EXIT], event, ELEMS_COUNT(event));

    AsciiToGxtChar("CONVERSATION_NO", event);
    CMessages::GxtStringCopy(m_eventNames[CONVERSATION_NO], event, ELEMS_COUNT(event));
    AsciiToGxtChar("CONVERSATION_YES", event);
    CMessages::GxtStringCopy(m_eventNames[CONVERSATION_YES], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GROUP_CONTROL_FWD", event);
    CMessages::GxtStringCopy(m_eventNames[GROUP_CONTROL_FORWARDS], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GROUP_CONTROL_BWD", event);
    CMessages::GxtStringCopy(m_eventNames[GROUP_CONTROL_BACK], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GO_LEFT", event);
    CMessages::GxtStringCopy(m_eventNames[LEFT], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GO_RIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[RIGHT], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GO_FORWARD", event);
    CMessages::GxtStringCopy(m_eventNames[FORWARDS], event, ELEMS_COUNT(event));
    AsciiToGxtChar("GO_BACK", event);
    CMessages::GxtStringCopy(m_eventNames[BACKWARDS], event, ELEMS_COUNT(event));
    AsciiToGxtChar("VEHICLE_TURRETLEFT", event);
    CMessages::GxtStringCopy(m_eventNames[SPECIAL_CONTROL_LEFT], event, ELEMS_COUNT(event));
    AsciiToGxtChar("VEHICLE_TURRETRIGHT", event);
    CMessages::GxtStringCopy(m_eventNames[SPECIAL_CONTROL_RIGHT], event, ELEMS_COUNT(event));
    AsciiToGxtChar("VEHICLE_TURRETUP", event);
    CMessages::GxtStringCopy(m_eventNames[SPECIAL_CONTROL_DOWN], event, ELEMS_COUNT(event));
    AsciiToGxtChar("VEHICLE_TURRETDOWN", event);
    CMessages::GxtStringCopy(m_eventNames[SPECIAL_CONTROL_UP], event, ELEMS_COUNT(event));
    AsciiToGxtChar("NETWORK_TALK", event);
    CMessages::GxtStringCopy(m_eventNames[NETWORK_TALK], event, ELEMS_COUNT(event));

    AsciiToGxtChar("TOGGLE_DPAD", event);
    CMessages::GxtStringCopy(m_eventNames[TOGGLE_DPAD], event, ELEMS_COUNT(event));

    AsciiToGxtChar("SWITCH_DEBUG_CAM_ON", event);
    CMessages::GxtStringCopy(m_eventNames[DEBUG_CAM_ON], event, ELEMS_COUNT(event));

    AsciiToGxtChar("TAKE_SCREEN_SHOT", event);
    CMessages::GxtStringCopy(m_eventNames[TAKE_SCREEN_SHOT], event, ELEMS_COUNT(event));
}

bool CControllerConfigManager::GetIsKeyboardKeyDown(RsKeyCodes key)
{
	if (key < 255 && currKeyState.standardKeys[key])
	{
		return true;
	}
	for (size_t i = 0; i < 12; i++)
	{
		if (rsF1 + i == key)
		{
			CPad::GetPad(0);
			if (currKeyState.FKeys[i])
			{
				return true;
			}
		}
	}
    switch (key)
    {
		case rsESC: return currKeyState.esc;
		case rsINSERT: return currKeyState.insert;
		case rsDEL: return currKeyState.del;
		case rsHOME: return currKeyState.home;
		case rsEND: return currKeyState.end;
		case rsPGUP: return currKeyState.pgup;
		case rsPGDN: return currKeyState.pgdn;
		case rsUP: return currKeyState.up;
		case rsDOWN: return currKeyState.down;
		case rsLEFT: return currKeyState.left;
		case rsRIGHT: return currKeyState.right;
		case rsSCROLL: return currKeyState.scroll;
		case rsPAUSE: return currKeyState.pause;
		case rsNUMLOCK: return currKeyState.numlock;
		case rsDIV: return currKeyState.div;
		case rsMUL: return currKeyState.mul;
		case rsSUB: return currKeyState.sub;
		case rsADD: return currKeyState.add;
		case rsENTER: return currKeyState.enter;
		case rsEXTDEL: return currKeyState.decimal;
		case rsEXT_END: return currKeyState.num1;
		case rsEXTDOWN: return currKeyState.num2;
		case rsEXTPGDN: return currKeyState.num3;
		case rsEXTLEFT: return currKeyState.num4;
		case rsNUM5: return currKeyState.num5;
		case rsEXTRIGHT: return currKeyState.num6;
		case rsEXTHOME: return currKeyState.num7;
		case rsEXTUP: return currKeyState.num8;
		case rsEXTPGUP: return currKeyState.num9;
		case rsEXTINSERT: return currKeyState.num0;
		case rsBACK: return currKeyState.back;
		case rsTAB: return currKeyState.tab;
		case rsCAPSLOCK: return currKeyState.capslock;
		case rsEXTENTER: return currKeyState.extenter;
		case rsLSHIFT: return currKeyState.lshift;
		case rsSHIFT: return currKeyState.shift;
		case rsRSHIFT: return currKeyState.rshift;
		case rsLCTRL: return currKeyState.lctrl;
		case rsRCTRL: return currKeyState.rctrl;
		case rsLMENU: return currKeyState.lmenu;
		case rsRMENU: return currKeyState.rmenu;
		case rsLWIN: return currKeyState.lwin;
		case rsRWIN: return currKeyState.rwin;
		case rsAPPS: return currKeyState.apps;
		default:
			return false;
	}
}

bool CControllerConfigManager::GetIsKeyboardKeyJustDown(RsKeyCodes key)
{
	if (key < 255 && currKeyState.FKeys[key] && !prevKeyState.FKeys[key])
	{
		return true;
	}
	for (size_t i = 0; i < 12; i++)
	{
		if (rsF1 + i == key && currKeyState.FKeys[i] && !prevKeyState.FKeys[i])
		{
			return true;
		}
	}
	switch (key)
	{
		case rsESC: return currKeyState.esc && !prevKeyState.esc;
		case rsINSERT: return currKeyState.insert && !prevKeyState.insert;
		case rsDEL: return currKeyState.del && !prevKeyState.del;
		case rsHOME: return currKeyState.home && !prevKeyState.home;
		case rsEND: return currKeyState.end && !prevKeyState.end;
		case rsPGUP: return currKeyState.pgup && !prevKeyState.pgup;
		case rsPGDN: return currKeyState.pgdn && !prevKeyState.pgdn;
		case rsUP: return currKeyState.up && !prevKeyState.up;
		case rsDOWN: return currKeyState.down && !prevKeyState.down;
		case rsLEFT: return currKeyState.left && !prevKeyState.left;
		case rsRIGHT: return currKeyState.right && !prevKeyState.right;
		case rsSCROLL: return currKeyState.scroll && !prevKeyState.scroll;
		case rsPAUSE: return currKeyState.pause && !prevKeyState.pause;
		case rsNUMLOCK: return currKeyState.numlock && !prevKeyState.numlock;
		case rsDIV: return currKeyState.div && !prevKeyState.div;
		case rsMUL: return currKeyState.mul && !prevKeyState.mul;
		case rsSUB: return currKeyState.sub && !prevKeyState.sub;
		case rsADD: return currKeyState.add && !prevKeyState.add;
		case rsENTER: return currKeyState.enter && !prevKeyState.enter;
		case rsEXTDEL: return currKeyState.decimal && !prevKeyState.decimal;
		case rsEXT_END: return currKeyState.num1 && !prevKeyState.num1;
		case rsEXTDOWN: return currKeyState.num2 && !prevKeyState.num2;
		case rsEXTPGDN: return currKeyState.num3 && !prevKeyState.num3;
		case rsEXTLEFT: return currKeyState.num4 && !prevKeyState.num4;
		case rsNUM5: return currKeyState.num5 && !prevKeyState.num5;
		case rsEXTRIGHT: return currKeyState.num6 && !prevKeyState.num6;
		case rsEXTHOME: return currKeyState.num7 && !prevKeyState.num7;
		case rsEXTUP: return currKeyState.num8 && !prevKeyState.num8;
		case rsEXTPGUP: return currKeyState.num9 && !prevKeyState.num9;
		case rsEXTINSERT: return currKeyState.num0 && !prevKeyState.num0;
		case rsBACK: return currKeyState.back && !prevKeyState.back;
		case rsTAB: return currKeyState.tab && !prevKeyState.tab;
		case rsCAPSLOCK: return currKeyState.capslock && !prevKeyState.capslock;
		case rsEXTENTER: return currKeyState.extenter && !prevKeyState.extenter;
		case rsLSHIFT: return currKeyState.lshift && !prevKeyState.lshift;
		case rsSHIFT: return currKeyState.shift && !prevKeyState.shift;
		case rsRSHIFT: return currKeyState.rshift && !prevKeyState.rshift;
		case rsLCTRL: return currKeyState.lctrl && !prevKeyState.lctrl;
		case rsRCTRL: return currKeyState.rctrl && !prevKeyState.rctrl;
		case rsLMENU: return currKeyState.lmenu && !prevKeyState.lmenu;
		case rsRMENU: return currKeyState.rmenu && !prevKeyState.rmenu;
		case rsLWIN: return currKeyState.lwin && !prevKeyState.lwin;
		case rsRWIN: return currKeyState.rwin && !prevKeyState.rwin;
		case rsAPPS: return currKeyState.apps && !prevKeyState.apps;
		default: return false;
    }
}

bool CControllerConfigManager::GetIsMouseButtonDown(RsKeyCodes button)
{
	switch (button)
	{
		case 1: return CPad::NewMouseControllerState.lmb;
		case 2: return CPad::NewMouseControllerState.mmb;
		case 3: return CPad::NewMouseControllerState.rmb;
		case 4: return CPad::NewMouseControllerState.wheelUp;
		case 5: return CPad::NewMouseControllerState.wheelDown;
		case 6: return CPad::NewMouseControllerState.bmx1;
		case 7: return CPad::NewMouseControllerState.bmx2;
		default: return false;
	}
}

bool CControllerConfigManager::GetIsMouseButtonUp(RsKeyCodes button)
{
	switch (button)
	{
		case 1: return !CPad::NewMouseControllerState.lmb;
		case 2: return !CPad::NewMouseControllerState.mmb;
		case 3: return !CPad::NewMouseControllerState.rmb;
		case 4: return !CPad::NewMouseControllerState.wheelUp;
		case 5: return !CPad::NewMouseControllerState.wheelDown;
		case 6: return !CPad::NewMouseControllerState.bmx1;
		case 7: return !CPad::NewMouseControllerState.bmx2;
		default: return false;
	}
}

bool CControllerConfigManager::GetIsMouseButtonJustUp(RsKeyCodes button)
{
    switch (button)
    {
        case 1: return !CPad::NewMouseControllerState.lmb && (_BYTE)prevMouseState.lmb;
        case 2: return !CPad::NewMouseControllerState.rmb && prevMouseState.rmb;
        case 3: return !CPad::NewMouseControllerState.mmb && prevMouseState.mmb;
        case 4: return !CPad::NewMouseControllerState.wheelUp && prevMouseState.wheelUp;
        case 5: return !CPad::NewMouseControllerState.wheelDown && prevMouseState.wheelDown;
        case 6: return !CPad::NewMouseControllerState.bmx1 && prevMouseState.bmx1;
        case 7: return !CPad::NewMouseControllerState.bmx2 && prevMouseState.bmx2;
        default: return false;
    }
}

bool CControllerConfigManager::GetIsKeyBlank(int key, eControllerType controllerType)
{
	switch (controllerType)
	{
		case 0:
		case 1: 
			return key == rsNULL;
		case 2:
		case 3: 
			return !key;
		default: return true;
	}
}

int CControllerConfigManager::GetActionType(e_ControllerAction action)
{
	switch (action)
	{
		case CHANGE_CAMERA:
		case NETWORK_TALK:
		case TOGGLE_DPAD:
		case DEBUG_CAM_ON:
		case TAKE_SCREEN_SHOT:
		case TOGGLE_MOUSE_POINTER:
			return 4;
		case NEXT_WEAPON:
		case PREVIOUS_WEAPON:
		case JUMP:
		case SPRINT:
		case LOOK_BEHIND:
		case CROUCH:
		case ACTION:
		case WALK:
		case CYCLE_TARGET_LEFT:
		case CYCLE_TARGET_RIGHT:
		case CENTER_CAMERA_BEHIND_PLAYER:
		case CONVERSATION_YES:
		case CONVERSATION_NO:
		case GROUP_CONTROL_FORWARDS:
		case GROUP_CONTROL_BACK:
			return 1;
		case VEHICLE_FIRE:
		case VEHICLE_SECONDARY_FIRE:
		case VEHICLE_LEFT:
		case VEHICLE_RIGHT:
		case STEER_FORWARDS_DOWN:
		case STEER_BACK_UP:
		case ACCELERATE:
		case BRAKE_REVERSE:
		case RADIO_NEXT:
		case RADIO_PREVIOUS:
		case RADIO_USER_TRACK_SKIP:
		case HORN:
		case SUB_MISSION:
		case HANDBRAKE:
		case VEHICLE_LOOK_LEFT:
		case VEHICLE_LOOK_RIGHT:
		case VEHICLE_LOOK_BEHIND:
		case VEHICLE_MOUSE_LOOK:
		case SPECIAL_CONTROL_LEFT:
		case SPECIAL_CONTROL_RIGHT:
		case SPECIAL_CONTROL_DOWN:
		case SPECIAL_CONTROL_UP:
			return 2;
		case ENTER_EXIT:
			return 3;
		case FIRE:
		case FIRE_ALT:
		case FORWARDS:
		case BACKWARDS:
		case LEFT:
		case RIGHT:
		case ZOOM_IN:
		case ZOOM_OUT:
		case FIRST_PERSON_LOOK_LEFT:
		case FIRST_PERSON_LOOK_RIGHT:
		case AIM_WEAPON:
		case FIRST_PERSON_LOOK_UP:
		case FIRST_PERSON_LOOK_DOWN:
			return 5;
		default: 
			return 6;
	}
}