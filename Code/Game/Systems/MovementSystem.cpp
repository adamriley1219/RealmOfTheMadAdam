#include "Game/Systems/MovementSystem.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"

#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/RenderComp.hpp"

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
			Transform2D& transform = tuple->trans_comp->m_transform;
			PhysicsComp& phyx = *tuple->phys_comp;
			IntentComp& intent = *tuple->intent_comp;

			float change_vel = phyx.m_acceleration * deltaTime;
			float frame_friction = phyx.m_friction_per_sec * deltaTime;

			phyx.m_velocity += intent.m_desired_move_direction * change_vel;
			phyx.m_velocity *= Clamp( 1.0f - frame_friction, 0.0f, 1.0f );
			phyx.m_velocity.ClampLength( phyx.m_max_speed );

			transform.m_position += phyx.m_velocity * deltaTime;

			RenderComp* render_comp = (RenderComp*)ent_pair.second->GetComponent( RENDER_COMP );
			if( render_comp )
			{
				std::vector<Vertex_PCU>& verts = render_comp->m_verts_groups[""].verts;
				AddVertsForRing2D( verts, Vec2::ZERO, .25f, .01f, Rgba::WHITE, 8 );
				
				render_comp->m_verts_groups[""].transform = transform;
			}
		}
		SAFE_DELETE(tuple);
	}
}
