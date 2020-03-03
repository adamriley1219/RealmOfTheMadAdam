#include "Game/Systems/TriggerSystem.hpp"
#include "Game/EntityAdmin.hpp"

#include "Game/Components/TriggerComp.hpp"
#include "Game/Components/StatsComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"

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
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{
		if (!entity.m_claimed)
		{
			continue;
		}
		
		TriggerComp* trigger_comp	= (TriggerComp*) entity.GetComponent( TRIGGER_COMP );
		PhysicsComp* phyx_comp		= (PhysicsComp*) entity.GetComponent( PHYSICS_COMP );
		TransformComp* trans_comp	= (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		StatsComp* stats_comp		= (StatsComp*) entity.GetComponent( STATS_COMP );
		if( trigger_comp && phyx_comp && trans_comp && stats_comp )
		{
			if( trigger_comp->m_isTrigger )
			{
				// trigger's don't trigger other triggers.
				continue;
			}

			for( Entity& other_entity : admin.m_entities )
			{
				if (!other_entity.m_claimed)
				{
					continue;
				}
				TriggerComp* other_trigger_comp = (TriggerComp*) other_entity.GetComponent( TRIGGER_COMP );

				if( other_trigger_comp && other_trigger_comp->m_isTrigger )
				{
					PhysicsComp* other_phyx_comp = (PhysicsComp*) other_entity.GetComponent( PHYSICS_COMP );
					TransformComp* other_trans_comp = (TransformComp*) other_entity.GetComponent( TRANSFORM_COMP );
					if( other_phyx_comp && other_trans_comp )
					{
						bool is_in_range = DoDiscsOverlap( trans_comp->m_transform.m_position, phyx_comp->m_radius,
							other_trans_comp->m_transform.m_position, other_phyx_comp->m_radius );
						if( is_in_range )
						{
							// The other trigger is in range of the current entity none trigger entity
							StatsComp* other_stats_comp = (StatsComp*) other_entity.GetComponent( STATS_COMP );
							if( other_stats_comp && stats_comp->m_team != other_stats_comp->m_team )
							{
								stats_comp->m_health -= other_trigger_comp->m_damage_on_triggered;
								if (other_trigger_comp->m_die_on_contect)
								{
									other_entity.m_destroy = true;
								}
							}
						}
					}
				}
			}
		}
	}
}
