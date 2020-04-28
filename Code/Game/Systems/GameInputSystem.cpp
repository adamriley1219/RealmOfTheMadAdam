#include "Game/Systems/GameInputSystem.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Game.hpp"

#include "Game/Components/IntentComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/RenderComp.hpp"

#include "Engine/Core/WindowContext.hpp"
#include "Engine/Renderer/Camera.hpp"

#include "Engine/Memory/Debug/LogProfile.hpp"

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
	UNUSED(deltaTime);
	EntityAdmin& admin = GetCurrentAdmin();
	Entity* player =  admin.GetFirstWithComp( INPUT_COMP );
	if( !player )
	{
		return;
	}

	InputComp* input_comp_ptr = (InputComp*)player->GetComponent(INPUT_COMP);
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


		IntentComp* intent_comp_ptr = (IntentComp*)player->GetComponent( INTENT_COMP );
		if( intent_comp_ptr )
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


			intent_comp_ptr->m_wants_to_interact = input_comp.GetKeyState(INTERACT_HOTKEY)->WasJustPressed();
			intent_comp_ptr->m_has_ported = false;

			intent_comp_ptr->m_wants_to_fire = input_comp.GetKeyState(FIRE_HOTKEY)->IsPressed();
			intent_comp_ptr->m_wants_to_fire_secondary = input_comp.GetKeyState(FIRE_SECONDARY_HOTKEY)->IsPressed();

			intent_comp_ptr->m_desired_move_direction = dir.GetNormalized();;

			TransformComp* trans_comp = (TransformComp*) player->GetComponent( TRANSFORM_COMP );
			CameraComp* camera_comp = (CameraComp*) player->GetComponent( CAMERA_COMP );
			if( trans_comp && ( intent_comp_ptr->m_wants_to_fire || intent_comp_ptr->m_wants_to_fire_secondary ) )
			{
				IntVec2 raw_mouse_pos = g_theWindowContext->GetClientMousePosition();
				Vec3 world_pos = camera_comp->m_camera->GetClientToWorld( raw_mouse_pos );

				intent_comp_ptr->m_desired_fire_direction = ( Vec2( world_pos  ) - trans_comp->m_transform.m_position ).GetNormalized();
			}
		}
	}

	RenderComp* render_comp_ptr = (RenderComp*)player->GetComponent( RENDER_COMP );
	if( render_comp_ptr )
	{
		bool inGodMode = g_theGame->IsInGodMode();
		std::vector<Vertex_PCU>& verts = render_comp_ptr->m_main_group.verts;
		if( inGodMode )
		{
			for( Vertex_PCU& vert : verts )
			{
				vert.color = Rgba::YELLOW;
			}
		}
		else
		{
			for (Vertex_PCU& vert : verts)
			{
				vert.color = Rgba::WHITE;
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