#include "Game/Systems/GameInputSystem.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Components/IntentComp.hpp"

//--------------------------------------------------------------------------
/**
* GameInpuSystem
*/
GameInputSystem::GameInputSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~GameInpuSystem
*/
GameInputSystem::~GameInputSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void GameInputSystem::Update( float deltaTime ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{

		InputComp* input_comp_ptr = (InputComp*)entity.GetComponent(INPUT_COMP);
		if( input_comp_ptr )
		{
			InputComp& input_comp = *input_comp_ptr;
			input_comp.GetKeyState(MOVE_LEFT_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(MOVE_LEFT_HOTKEY)));
			input_comp.GetKeyState(MOVE_RIGHT_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(MOVE_RIGHT_HOTKEY)));
			input_comp.GetKeyState(MOVE_DOWN_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(MOVE_DOWN_HOTKEY)));
			input_comp.GetKeyState(MOVE_UP_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(MOVE_UP_HOTKEY)));
			input_comp.GetKeyState(FIRE_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(FIRE_HOTKEY)));
			input_comp.GetKeyState(FIRE_SECONDARY_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(FIRE_SECONDARY_HOTKEY)));
			input_comp.GetKeyState(INTERACT_HOTKEY)->UpdateStatus(g_theInputSystem->KeyIsDown(GetKeyFromHotkey(INTERACT_HOTKEY)));


			IntentComp* interact_comp_ptr = (IntentComp*)entity.GetComponent( INTENT_COMP );
			if( interact_comp_ptr )
			{
				Vec2 dir(0, 0);
				if (input_comp.GetKeyState(MOVE_LEFT_HOTKEY)->IsPressed())
				{
					dir.x -= 1.0f;
				}
				if (input_comp.GetKeyState(MOVE_UP_HOTKEY)->IsPressed())
				{
					dir.y += 1.0f;
				}
				if (input_comp.GetKeyState(MOVE_DOWN_HOTKEY)->IsPressed())
				{
					dir.y -= 1.0f;
				}
				if (input_comp.GetKeyState(MOVE_RIGHT_HOTKEY)->IsPressed())
				{
					dir.x += 1.0f;
				}

				interact_comp_ptr->m_wants_to_interact = input_comp.GetKeyState(INTERACT_HOTKEY)->IsPressed();
				interact_comp_ptr->m_wants_to_fire = input_comp.GetKeyState(FIRE_HOTKEY)->IsPressed();
				interact_comp_ptr->m_wants_to_fire_secondary = input_comp.GetKeyState(FIRE_SECONDARY_HOTKEY)->IsPressed();
				interact_comp_ptr->m_desired_move_direction = dir.GetNormalized();;
			}
		}
		
	}
}

//--------------------------------------------------------------------------
/**
* GetKeyFromHotkey
*/
Key GameInputSystem::GetKeyFromHotkey( eHotkeys hotkey ) const
{
	switch (hotkey)
	{
	case INTERACT_HOTKEY:
		return KEY_F;
		break;
	case FIRE_HOTKEY:
		return MOUSE_L;
		break;
	case FIRE_SECONDARY_HOTKEY:
		return MOUSE_R;
		break;
	case MOVE_LEFT_HOTKEY:
		return KEY_A;
		break;
	case MOVE_RIGHT_HOTKEY:
		return KEY_D;
		break;
	case MOVE_UP_HOTKEY:
		return KEY_W;
		break;
	case MOVE_DOWN_HOTKEY:
		return KEY_S;
		break;
	default:
		return KEY_INVALID;
		break;
	}
}