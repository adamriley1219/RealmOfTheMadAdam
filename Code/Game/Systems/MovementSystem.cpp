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

#include "Engine/Core/Debug/DevConsole.hpp"
#include "Engine/Core/EngineCommon.hpp"


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
	for( Entity& entity : admin.m_entities )
	{
		if (!entity.m_claimed)
		{
			continue;
		}

		TransformComp* trans_comp = (TransformComp*)admin.GetComponent(entity.m_id, TRANSFORM_COMP);
		PhysicsComp* physics_comp = (PhysicsComp*)admin.GetComponent(entity.m_id, PHYSICS_COMP);
		IntentComp* intent_comp = (IntentComp*)admin.GetComponent(entity.m_id, INTENT_COMP);
		if ( trans_comp && physics_comp && intent_comp && !physics_comp->m_static_object )
		{
			Transform2D& transform = trans_comp->m_transform;
			PhysicsComp& phyx = *physics_comp;
			IntentComp& intent = *intent_comp;

			float frame_change_speed = phyx.m_acceleration * deltaTime;
			float frame_friction = phyx.m_friction_per_sec * deltaTime;

			phyx.m_velocity += intent.m_desired_move_direction * frame_change_speed;
			phyx.m_velocity *= Clamp(1.0f - frame_friction, 0.0f, 1.0f);
			phyx.m_velocity.ClampLength(phyx.m_max_speed);

			transform.m_position += phyx.m_velocity * deltaTime;
			
			// apply push out from physics collision work
			transform.m_position += phyx.m_push_out_dir_amount;
		}


		RenderComp* render_comp = (RenderComp*)entity.GetComponent(RENDER_COMP);
		if (render_comp && trans_comp && physics_comp)
		{
			Transform2D& transform = trans_comp->m_transform;
			std::vector<Vertex_PCU>& verts = render_comp->m_verts_groups[""].verts;
			AddVertsForRing2D(verts, trans_comp->m_transform.m_position, physics_comp->m_radius, .01f, Rgba::WHITE, 8);
		}
	}
}
