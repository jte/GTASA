#pragma once

enum e_ControllerAction
{
    FIRE,
    FIRE_ALT,
    NEXT_WEAPON,
    PREVIOUS_WEAPON,
    FORWARDS,
    BACKWARDS,
    LEFT,
    RIGHT,
    ZOOM_IN,
    ZOOM_OUT,
    ENTER_EXIT,
    CHANGE_CAMERA,
    JUMP,
    SPRINT,
    LOOK_BEHIND,
    CROUCH,
    ACTION,
    WALK,
    VEHICLE_FIRE,
    VEHICLE_SECONDARY_FIRE,
    VEHICLE_LEFT,
    VEHICLE_RIGHT,
    STEER_FORWARDS_DOWN,
    STEER_BACK_UP,
    ACCELERATE,
    BRAKE_REVERSE,
    RADIO_NEXT,
    RADIO_PREVIOUS,
    RADIO_USER_TRACK_SKIP,
    HORN,
    SUB_MISSION,
    HANDBRAKE,
    FIRST_PERSON_LOOK_LEFT,
    FIRST_PERSON_LOOK_RIGHT,
    VEHICLE_LOOK_LEFT,
    VEHICLE_LOOK_RIGHT,
    VEHICLE_LOOK_BEHIND,
    VEHICLE_MOUSE_LOOK,
    SPECIAL_CONTROL_LEFT,
    SPECIAL_CONTROL_RIGHT,
    SPECIAL_CONTROL_DOWN,
    SPECIAL_CONTROL_UP,
    CYCLE_TARGET_LEFT,
    CYCLE_TARGET_RIGHT,
    CENTER_CAMERA_BEHIND_PLAYER,
    AIM_WEAPON,
    NETWORK_TALK,
    CONVERSATION_YES,
    CONVERSATION_NO,
    GROUP_CONTROL_FORWARDS,
    GROUP_CONTROL_BACK,
    FIRST_PERSON_LOOK_UP,
    FIRST_PERSON_LOOK_DOWN,
    TOGGLE_DPAD = 54,
    DEBUG_CAM_ON,
    TAKE_SCREEN_SHOT,
    TOGGLE_MOUSE_POINTER
};

enum eControllerType
{ // [0,3]
};

enum e_ControllerAction
{ // [0,5]
};

class CControllerConfigManager
{
public:
	/* AffectControllerStateOn_ButtonDown(int, eControllerType)
	* AffectControllerStateOn_ButtonDown_AllStates(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonDown_DebugStuff(int, eControllerType)
	* AffectControllerStateOn_ButtonDown_Driving(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonDown_FirstAndThirdPersonOnly(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonDown_FirstPersonOnly(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonDown_ThirdPersonOnly(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonDown_VehicleAndThirdPersonOnly(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonUp(int, eControllerType)
	* AffectControllerStateOn_ButtonUp_All_Player_States(int, eControllerType, CControllerState&)
	* AffectControllerStateOn_ButtonUp_DebugStuff(int, eControllerType)
	* AffectPadFromKeyBoard()
	* AffectPadFromMouse()
	* ClearSettingsAssociatedWithAction(e_ControllerAction, eControllerType)
	* ClearSimButtonPressCheckers()
	* DeleteMatching1rst3rdPersonControls(e_ControllerAction, int, eControllerType)
	* DeleteMatching1rstPersonControls(e_ControllerAction, int, eControllerType)
	* DeleteMatching3rdPersonControls(e_ControllerAction, int, eControllerType)
	* DeleteMatchingActionInitiators(e_ControllerAction, int, eControllerType)
	* DeleteMatchingCommonControls(e_ControllerAction, int, eControllerType)
	* DeleteMatchingVehicleControls(e_ControllerAction, int, eControllerType)
	* DeleteMatchingVehicle_3rdPersonControls(e_ControllerAction, int, eControllerType)
	*/ 
	static int GetActionType(e_ControllerAction action);
	/* GetButtonComboText(e_ControllerAction)
	* GetControllerKeyAssociatedWithAction(e_ControllerAction, eControllerType)
	* GetControllerSettingTextJoystick(e_ControllerAction)
	* GetControllerSettingTextKeyBoard(e_ControllerAction, eControllerType)
	* GetControllerSettingTextMouse(e_ControllerAction)
	* GetControllerSettingTextWithOrderNumber(e_ControllerAction, eContSetOrder)
	* GetFirstKeyboardControllerText(e_ControllerAction)
	* GetGxtStringOfCommandKeys(unsigned short, unsigned short*, unsigned short)
	* GetIsActionAButtonCombo(e_ControllerAction)
	*/ 
	static bool GetIsKeyBlank(int key, eControllerType controllerType);
	static bool GetIsKeyboardKeyDown(RsKeyCodes key);
	static bool GetIsKeyboardKeyJustDown(RsKeyCodes key);
	static bool GetIsMouseButtonDown(RsKeyCodes button);
	static bool GetIsMouseButtonJustUp(RsKeyCodes button);
	static bool GetIsMouseButtonUp(RsKeyCodes button);
	/* GetJoyButtonJustDown()
	* GetJoyButtonJustUp()
	* GetMouseButtonAssociatedWithAction(e_ControllerAction)
	* GetNumOfSettingsForAction(e_ControllerAction)
	* InitDefaultControlConfigJoyPad(unsigned int)
	* InitDefaultControlConfigMouse(CMouseControllerState const&, bool)
	* InitDefaultControlConfiguration()
	*/
    static void InitialiseControllerActionNameArray();
	/* LoadSettings(int)
	* MakeControllerActionsBlank()
	* ReinitControls()
	* ResetSettingOrder(e_ControllerAction)
	* SaveSettings(int)
	* SetControllerKeyAssociatedWithAction(e_ControllerAction, int, eControllerType)
	* SetMouseButtonAssociatedWithAction(e_ControllerAction, int)
	* StoreMouseButtonState(eMouseButtons, bool)
	* UpdateJoyButtonState(int)
	* UpdateJoyInConfigMenus_ButtonDown(int, int)
	* UpdateJoyInConfigMenus_ButtonUp(int, int)
    */
private:
    static uint16_t m_eventNames[59][40];
};