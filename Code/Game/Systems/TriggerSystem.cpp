#include "Game/Systems/TriggerSystem.hpp"
#include "Game/EntityAdmin.hpp"
#include "Game/Map.hpp"

#include "Game/Components/TriggerComp.hpp"
#include "Game/Components/StatsComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/IntentComp.hpp"

#include "Engine/Memory/Debug/LogProfile.hpp"


#include "Game/Game.hpp"

//--------------------------------------------------------------------------
/**
* TriggerSystem
*/
TriggerSystem::TriggerSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~TriggerSystem
*/
TriggerSystem::~TriggerSystem()
{
	
}

//--------------------------------------------------------------------------
/**
* Update
*/
void TriggerSystem::Update( float deltaTime ) const
{
	UNUSED(deltaTime);
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity* trigger : admin.GetAllWithComp( TRIGGER_COMP ) )
	{
		for( Entity* entity : admin.GetAllWithComp( TRANSFORM_COMP ) )
		{
			UpdateTriggerOnEntity( *trigger, *entity );
		}
	}


	for( Entity& entity : admin.m_entities )
	{
		if (!entity.m_claimed)
		{
			continue;
		}
		
	
	}
}

//--------------------------------------------------------------------------
/**
* UpdateTriggerOnEntity
* If the other entity is a trigger (owns one) then skip.
*/
void TriggerSystem::UpdateTriggerOnEntity( Entity& trigger, Entity& entity ) const
{
	TriggerComp* entity_trigger_comp = (TriggerComp*)entity.GetComponent(TRIGGER_COMP);
	if( entity_trigger_comp )
	{
		// Don't trigger off other triggers
		return;
	}

	TriggerComp* trigger_comp = (TriggerComp*)trigger.GetComponent(TRIGGER_COMP);
	PhysicsComp* trigger_phyx_comp = (PhysicsComp*)trigger.GetComponent(PHYSICS_COMP);
	TransformComp* trigger_trans_comp = (TransformComp*)trigger.GetComponent(TRANSFORM_COMP);
	StatsComp* trigger_stats_comp = (StatsComp*)trigger.GetComponent(STATS_COMP);

	PhysicsComp* entity_phyx_comp = (PhysicsComp*)entity.GetComponent(PHYSICS_COMP);
	TransformComp* entity_trans_comp = (TransformComp*)entity.GetComponent(TRANSFORM_COMP);
	StatsComp* entity_stats_comp = (StatsComp*)entity.GetComponent(STATS_COMP);
	IntentComp* entity_intent_comp = (IntentComp*)entity.GetComponent(INTENT_COMP);
	
	if( trigger_phyx_comp && trigger_trans_comp && trigger_comp
		&& entity_phyx_comp && entity_trans_comp )
	{
		Vec2& entity_position = entity_trans_comp->m_transform.m_position;

		bool is_in_range = DoDiscsOverlap( trigger_trans_comp->m_transform.m_position, trigger_phyx_comp->m_radius,
			entity_position, entity_phyx_comp->m_radius );
		if( is_in_range )
		{
			// Check to interact with stats
			if( entity_stats_comp && trigger_stats_comp )
			{
				if ( trigger_stats_comp->m_team != entity_stats_comp->m_team )
				{
					bool inGodMode = g_theGame->IsInGodMode();
					if( inGodMode )
					{
						// Instantly kill enemies only.
						if( entity_stats_comp->m_team == ENEMY_TEAM )
						{
							entity_stats_comp->m_health = 0;
						}
					}
					else
					{
						entity_stats_comp->m_health -= trigger_comp->m_damage_on_triggered;
					}

					if (trigger_comp->m_die_on_contect)
					{
						trigger.m_destroy = true;
					}
				}
			}

			if( entity_intent_comp && entity_intent_comp->m_wants_to_interact && !entity_intent_comp->m_has_ported )
			{
				// Check to interact with portal
				if( trigger_comp->m_portal_active )
				{
					entity_position = trigger_comp->m_portal_to_position;
					entity_intent_comp->m_has_ported = true;
				}

				// Check to change map
				if( trigger_comp->m_transfer_map  )
				{
					entity.m_map_to_transfer_to = trigger_comp->m_map_to_transfer;
					entity_intent_comp->m_has_ported = true;
				}
			}
		}
		
	}
}
