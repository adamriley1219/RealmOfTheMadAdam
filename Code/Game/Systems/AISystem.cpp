#include "Game/Systems/AISystem.hpp"

#include "Game/EntityAdmin.hpp"

#include "Game/Components/TransformComp.hpp"
#include "Game/Components/AIComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/TriggerComp.hpp"

#include "Engine/Math/MathUtils.hpp"

//--------------------------------------------------------------------------
/**
* UISystem
*/
AISystem::AISystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~UISystem
*/
AISystem::~AISystem()
{

}


//--------------------------------------------------------------------------
/**
* Update
*/
void AISystem::Update( float deltaTime ) const
{
	UNUSED(deltaTime);
	EntityAdmin& admin = GetCurrentAdmin();
	for (Entity& entity : admin.m_entities)
	{
		if (!entity.m_claimed)
		{
			continue;
		}

		AIComp* ai_comp = (AIComp*) entity.GetComponent( AI_COMP );
		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		StatsComp* stats_comp = (StatsComp*) entity.GetComponent( STATS_COMP );
		IntentComp* intent_comp = (IntentComp*) entity.GetComponent( INTENT_COMP );

		if( ai_comp && trans_comp && stats_comp )
		{
			Vec2& cur_ai_position = trans_comp->m_transform.m_position;

			Entity* found_entity = GetClosetsEnemyInRange( cur_ai_position, ai_comp->m_vision_radius, *stats_comp );
			if( found_entity )
			{
				TransformComp* other_trans_comp = (TransformComp*) found_entity->GetComponent( TRANSFORM_COMP );
				if( other_trans_comp )
				{
					// Found an enemy
					Vec2& other_pos = other_trans_comp->m_transform.m_position;

					intent_comp->m_wants_to_fire = true;

					Vec2 ai_to_found_entity = other_pos - cur_ai_position;


					ai_to_found_entity.Normalize();
					intent_comp->m_desired_fire_direction = ai_to_found_entity;

					intent_comp->m_desired_move_direction = ai_to_found_entity;
				}
			}
			else
			{
				// Can't see an enemy
				intent_comp->m_wants_to_fire = false;
				intent_comp->m_desired_move_direction = Vec2::ZERO;
			}
		}
	}
}

//--------------------------------------------------------------------------
/**
* GetClosetsEneityInRange
*/
Entity* AISystem::GetClosetsEnemyInRange( const Vec2& position, float range, const StatsComp& to_check_stats ) const
{
	EntityAdmin& admin = GetCurrentAdmin();

	Entity* best_entity = nullptr;
	float best_distance_squared = range * range;

	for (Entity& entity : admin.m_entities)
	{
		if (!entity.m_claimed)
		{
			continue;
		}

		TriggerComp* trigger_comp = (TriggerComp*) entity.GetComponent( TRIGGER_COMP );

		if( trigger_comp )
		{
			continue;
		}

		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		StatsComp* stats_comp = (StatsComp*) entity.GetComponent( STATS_COMP );

		if( trans_comp && stats_comp )
		{
			if( to_check_stats.IsEnemy( stats_comp->m_team ) )
			{
				float dist_squared = GetDistanceSquared( position, trans_comp->m_transform.m_position );
				if( dist_squared < best_distance_squared )
				{
					best_distance_squared = dist_squared;
					best_entity = &entity;
				}
			}
		}
	}

	return best_entity;
}
