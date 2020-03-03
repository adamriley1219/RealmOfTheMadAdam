#include "Game/Systems/CombatSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/AbilityComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/TriggerComp.hpp"
#include "Game/Components/StatsComp.hpp"

//--------------------------------------------------------------------------
/**
* CombatSystem
*/
CombatSystem::CombatSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~CombatSystem
*/
CombatSystem::~CombatSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void CombatSystem::Update( float delta_time ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{
		if( !entity.m_claimed )
		{
			continue;
		}

		IntentComp* intent = (IntentComp*) entity.GetComponent( INTENT_COMP );
		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		StatsComp* stats_comp = (StatsComp*) entity.GetComponent( STATS_COMP );
		if( intent && trans_comp && stats_comp )
		{
			if( !intent->m_fire_timer.HasElapsed() )
			{
				continue;
			}

			if( intent->m_wants_to_fire )
			{
				// Spawn bullet
				Entity* bullet = admin.CreateEntity();
				IntentComp* bullet_intent = (IntentComp*) bullet->AddComponent( INTENT_COMP );
				PhysicsComp* bullet_physics = (PhysicsComp*) bullet->AddComponent( PHYSICS_COMP );
				TriggerComp* bullet_trigger = (TriggerComp*) bullet->AddComponent( TRIGGER_COMP );
				TransformComp* bullet_transform = (TransformComp*) bullet->AddComponent( TRANSFORM_COMP );
				AbilityComp* bullet_ability = (AbilityComp*) bullet->AddComponent( ABILITY_COMP );
				RenderComp* bullet_render = (RenderComp*) bullet->AddComponent( RENDER_COMP );
				((StatsComp*) bullet->AddComponent( STATS_COMP ))->m_team = stats_comp->m_team;
				bullet_intent->m_desired_move_direction = intent->m_desired_fire_direction;
				bullet_intent->m_death_timer.SetAndReset( 0.75f );

				// Physics
				bullet_physics->m_radius = 0.15f;
				bullet_physics->m_static_object = false;
				bullet_physics->m_max_speed = 4.0f;
				bullet_physics->m_acceleration = 100.0f;

				// Trigger setup
				bullet_trigger->m_isTrigger = true;
				bullet_trigger->m_die_on_contect = true;
				bullet_trigger->m_damage_on_triggered = 2.0f;

				bullet_transform->m_transform = trans_comp->m_transform;
				
				intent->m_fire_timer.Reset();
			}
			if( intent->m_wants_to_fire_secondary )
			{
				intent->m_fire_timer.Reset();
			}
		}

		if( intent )
		{
			if( intent->m_death_timer.HasElapsed() )
			{
				entity.m_destroy = true;
			}
		}

		if( stats_comp )
		{
			if( stats_comp->m_health <= 0.0f )
			{
				entity.m_destroy = true;
			}
		}
	}
}
