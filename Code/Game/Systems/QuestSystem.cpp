#include "Game/Systems/QuestSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/InputComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/AIComp.hpp"
#include "Game/Components/StatsComp.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

#include "Engine/Core/Strings/NamedStrings.hpp"
#include "Engine/Core/Time/Clock.hpp"

//--------------------------------------------------------------------------
/**
* QuestSystem
*/
QuestSystem::QuestSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~QuestSystem
*/
QuestSystem::~QuestSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void QuestSystem::Update(float deltaTime) const
{
	UNUSED(deltaTime);
	EntityAdmin& admin = GetCurrentAdmin();
	
	Entity* carrier = (Entity*)admin.GetFirstWithComp( QUEST_CARRIER_COMP );
	if( carrier )
	{
		UpdateQuestCarrier( *carrier );

		for( Entity* giver : admin.GetAllWithComp( QUEST_GIVER_COMP ) )
		{
			// Can assume that giver is valid
			UpdateQuestGiverWithCarrier( *giver, *carrier );
		}
	}

}

//--------------------------------------------------------------------------
/**
* UpdateQuestGiver
*/
void QuestSystem::UpdateQuestGiverWithCarrier( Entity& giver, Entity& carrier ) const
{
	InteractComp* giver_interact_comp = (InteractComp*)giver.GetComponent(INTERACT_COMP);
	TransformComp* giver_trans_comp = (TransformComp*)giver.GetComponent(TRANSFORM_COMP);
	PhysicsComp* giver_physics_comp = (PhysicsComp*)giver.GetComponent(PHYSICS_COMP);
	RenderComp* giver_render_comp = (RenderComp*)giver.GetComponent(RENDER_COMP);
	QuestGiverComp* quest_giver_comp = (QuestGiverComp*)giver.GetComponent(QUEST_GIVER_COMP);
	QuestComp* quest = quest_giver_comp->GetCurrentQuest();

	QuestCarrierComp* carrier_comp = (QuestCarrierComp*)carrier.GetComponent(QUEST_CARRIER_COMP);
	IntentComp* carrier_intent_comp = (IntentComp*)carrier.GetComponent(INTENT_COMP);
	TransformComp* carrier_trans_comp = (TransformComp*)carrier.GetComponent(TRANSFORM_COMP);



	// Confirming required components
	if( giver_interact_comp && giver_trans_comp && giver_physics_comp && quest_giver_comp && giver_render_comp
		&& carrier_comp && carrier_trans_comp && carrier_intent_comp )
	{

		// Get displacement to player and check if the length squared is within range of the range squared
		Vec2 displcement = carrier_trans_comp->m_transform.m_position - giver_trans_comp->m_transform.m_position;
		bool in_range = displcement.GetLengthSquared() < giver_interact_comp->m_interact_range * giver_interact_comp->m_interact_range;

		if( in_range )
		{
			// Is in range of player
			if (carrier_intent_comp->m_wants_to_interact)
			{
				quest_giver_comp->being_interacted_with = true;
				if( quest )
				{
					if (quest->complete)
					{
						if( carrier_comp->HasQuest( quest ) )
						{
							QuestComplete( quest_giver_comp );
							carrier_comp->RemoveQuest( quest );
						}
					}
					else
					{
						if( !carrier_comp->HasQuest( quest ) )
						{
							carrier_comp->AddQuest( quest );
							QuestAccepted( quest_giver_comp );
						}
					}
				}
			}
		}
		else
		{
			quest_giver_comp->being_interacted_with = false;
		}


		// Add rendering elements according to state of quest
		if( quest )
		{
			float radius = giver_physics_comp->m_radius;

			if (quest->complete)
			{
				AddVertsForAABB2D(giver_render_comp->m_verts_groups["Data/Images/Extras_4x4.png"].verts, AABB2(radius, radius, giver_trans_comp->m_transform.m_position + Vec2::UP * .35f), Rgba::RED, Vec2(0.75f, 0.25f), Vec2(1.0f, 0.5f));
			}
			else
			{
				AddVertsForAABB2D(giver_render_comp->m_verts_groups["Data/Images/Extras_4x4.png"].verts, AABB2(radius, radius, giver_trans_comp->m_transform.m_position + Vec2::UP * .35f), Rgba::CYAN, Vec2(0.75f, 0.0f), Vec2(1.0f, 0.25f));
			}

			if( in_range )
			{
				// If in range to interact, render an F below the carrier.
				BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
				giver_render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
				font->AddVertsFor2DText(giver_render_comp->m_verts_groups["SquirrelFixedFont"].verts, Vec2(giver_trans_comp->m_transform.m_position) - Vec2(radius * .5f, radius * 4.0f) * .5f, .25f, "F", 0.5f, Rgba(1.0f, .9f, .6f));

			}
		}
	}
}

//--------------------------------------------------------------------------
/**
* UpdateQuestCarrier
*/
void QuestSystem::UpdateQuestCarrier( Entity& carrier ) const
{
	QuestCarrierComp* carrier_comp = (QuestCarrierComp*)carrier.GetComponent( QUEST_CARRIER_COMP );
	for( QuestComp* quest : carrier_comp->quests )
	{
		if( quest )
		{
			UpdateQuest( *quest );
		}
	}
}

//--------------------------------------------------------------------------
/**
* UpdateQuest
*/
void QuestSystem::UpdateQuest( QuestComp& quest_comp ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity* ai_entity : admin.GetAllWithComp( AI_COMP ) )
	{
		// update num of enemies that have dies if an enemy has died.
		StatsComp* other_ai_states = (StatsComp*)ai_entity->GetComponent( STATS_COMP );
		if( other_ai_states && ai_entity->m_claimed && ai_entity->m_destroy )
		{
			// ai was destroyed
			if( other_ai_states->m_team == ENEMY_TEAM )
			{
				// destroyed ai was an enemy
				if( quest_comp.num_enemies_killed < quest_comp.num_enemies_to_kill )
				{
					++quest_comp.num_enemies_killed;
				}
			}
		}
	}

	UpdateQuestStateWithTriggers( quest_comp );
}

//--------------------------------------------------------------------------
/**
* UpdateQuestState
*/
void QuestSystem::UpdateQuestStateWithTriggers( QuestComp& quest ) const
{
	if( quest.num_enemies_killed == quest.num_enemies_to_kill )
	{
		quest.complete = true;
	}
}

//--------------------------------------------------------------------------
/**
* TriggerQuestComplete
*/
void QuestSystem::QuestComplete( QuestGiverComp* giver ) const
{
	// #TODO: Trigger Quest Complete logic 
// 	giver->OnTriggeredComplete();
// 	quest;
// 
// 
// 	if( quest.ShouldOpenExit )
// 	{
// 		// Open exit comp for current map
// 	}
}

//--------------------------------------------------------------------------
/**
* TriggerQuestAccepted
*/
void QuestSystem::QuestAccepted( QuestGiverComp* giver ) const
{
	// #TODO: Trigger Quest Accepted logic 
}
