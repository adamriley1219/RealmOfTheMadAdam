#include "Game/Systems/MovementSystem.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Game/EntityAdmin.hpp"

#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/InputComp.hpp"

#include "Engine/Physics/Transform2D.hpp"


//--------------------------------------------------------------------------
/**
* GamePhysicsSystem
*/
MovementSystem::MovementSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~GamePhysicsSystem
*/
MovementSystem::~MovementSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void MovementSystem::Update( float deltaTime ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( auto ent_pair : admin.m_entities )
	{
		MovementTuple* tuple = admin.GetMovementTuple( ent_pair.first );
		if( tuple )
		{
			if( g_theGame->IsPlayer( ent_pair.first ) )
			{
				Transform2D& transform = tuple->trans_comp->m_transform;
				float speed = tuple->phys_comp->m_speed;
				InputComp* input = EntityAdmin::GetInputSingleton();

				transform.m_position += input->m_move_dir * speed * deltaTime;

			}
		}
	}
}
