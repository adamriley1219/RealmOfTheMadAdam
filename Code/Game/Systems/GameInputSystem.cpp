#include "Game/Systems/GameInputSystem.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Components/IntentComp.hpp"


//--------------------------------------------------------------------------
// Player hotkeys
//--------------------------------------------------------------------------
Key INTERACT_KEY = KEY_F;
Key FIRE_KEY = MOUSE_L;
Key FIRE_SECONDARY_KEY = MOUSE_R;
Key MOVE_LEFT = KEY_A;
Key MOVE_RIGHT = KEY_D;
Key MOVE_UP = KEY_W;
Key MOVE_DOWN = KEY_S;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
/**
* GameInpuSystem
*/
GameInpuSystem::GameInpuSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~GameInpuSystem
*/
GameInpuSystem::~GameInpuSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void GameInpuSystem::Update( float deltaTime ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( auto ent_pair : admin.m_entities )
	{
		InputTuple* tuple = admin.GetInputTuple( ent_pair.first );
		if( tuple )
		{
			InputComp& input_comp = *tuple->input_comp;
			input_comp.GetKeyState( MOVE_LEFT )->UpdateStatus( g_theInputSystem->KeyIsDown( MOVE_LEFT ) );
			input_comp.GetKeyState( MOVE_RIGHT )->UpdateStatus( g_theInputSystem->KeyIsDown( MOVE_RIGHT ) );
			input_comp.GetKeyState( MOVE_DOWN )->UpdateStatus( g_theInputSystem->KeyIsDown( MOVE_DOWN ) );
			input_comp.GetKeyState( MOVE_UP )->UpdateStatus( g_theInputSystem->KeyIsDown( MOVE_UP ) );
			input_comp.GetKeyState( FIRE_KEY )->UpdateStatus( g_theInputSystem->KeyIsDown( FIRE_KEY ) );
			input_comp.GetKeyState( FIRE_SECONDARY_KEY )->UpdateStatus( g_theInputSystem->KeyIsDown( FIRE_SECONDARY_KEY ) );
			input_comp.GetKeyState( INTERACT_KEY )->UpdateStatus( g_theInputSystem->KeyIsDown( INTERACT_KEY ) );

			Vec2 dir( 0, 0 );
			if( input_comp.GetKeyState( MOVE_LEFT )->IsPressed() )
			{
				dir.x -= 1.0f;
			}
			if( input_comp.GetKeyState( MOVE_UP )->IsPressed() )
			{
				dir.y += 1.0f;
			}
			if( input_comp.GetKeyState( MOVE_DOWN )->IsPressed() )
			{
				dir.y -= 1.0f;
			}
			if( input_comp.GetKeyState( MOVE_RIGHT )->IsPressed() )
			{
				dir.x += 1.0f;
			}

			tuple->intent_comp->m_wants_to_interact = input_comp.GetKeyState( INTERACT_KEY )->IsPressed();
			tuple->intent_comp->m_wants_to_fire = input_comp.GetKeyState( FIRE_KEY )->IsPressed();
			tuple->intent_comp->m_wants_to_fire_secondary = input_comp.GetKeyState( FIRE_SECONDARY_KEY )->IsPressed();
			tuple->intent_comp->m_desired_move_direction = dir.GetNormalized();;
		}
		SAFE_DELETE(tuple);
	}
}
