#include "Game/Systems/GameInputSystem.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Components/InputComp.hpp"

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
	InputComp* input_comp = EntityAdmin::GetInputSingleton();
	input_comp->GetKeyState( KEY_A )->UpdateStatus( g_theInputSystem->KeyIsDown( KEY_A ) );
	input_comp->GetKeyState( KEY_W )->UpdateStatus( g_theInputSystem->KeyIsDown( KEY_W ) );
	input_comp->GetKeyState( KEY_S )->UpdateStatus( g_theInputSystem->KeyIsDown( KEY_S ) );
	input_comp->GetKeyState( KEY_D )->UpdateStatus( g_theInputSystem->KeyIsDown( KEY_D ) );

	Vec2 dir( 0, 0 );
	if( input_comp->GetKeyState( KEY_A )->IsPressed() )
	{
		dir.x -= 1.0f;
	}
	if (input_comp->GetKeyState( KEY_W )->IsPressed())
	{
		dir.y += 1.0f;
	}
	if (input_comp->GetKeyState( KEY_S )->IsPressed())
	{
		dir.y -= 1.0f;
	}
	if (input_comp->GetKeyState( KEY_D )->IsPressed())
	{
		dir.x += 1.0f;
	}
	input_comp->m_move_dir = dir.GetNormalized();
}
