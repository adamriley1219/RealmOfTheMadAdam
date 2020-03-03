#pragma once
#include "Game/Components/Component.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"

enum eHotkeys
{
	UNKNOWN_HOTKEY = -1,
	INTERACT_HOTKEY,
	FIRE_HOTKEY,
	FIRE_SECONDARY_HOTKEY,
	MOVE_LEFT_HOTKEY,
	MOVE_RIGHT_HOTKEY,
	MOVE_UP_HOTKEY,
	MOVE_DOWN_HOTKEY,
	NUM_HOTKEYS
};

struct InputComp : public Component
{
public:
	InputComp();
	~InputComp();

	KeyButtonState* GetKeyState( eHotkeys key );

	virtual void Copy( const Component* copyFrom );
	virtual eComponentType GetType();
	virtual void Reset();

public:
	KeyButtonState m_button_states[NUM_HOTKEYS];

private:
	static InputComp s_prototype;


};
