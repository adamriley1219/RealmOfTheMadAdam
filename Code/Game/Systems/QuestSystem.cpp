#include "Game/Systems/QuestSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/InputComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/CameraComp.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/BitmapFont.hpp"

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
	for (auto entity : GetCurrentAdmin().m_entities)
	{
		if( !entity.m_claimed )
		{
			continue;
		}

		InteractComp* interact_comp = (InteractComp*)entity.GetComponent( INTERACT_COMP );
		TransformComp* trans_comp = (TransformComp*)entity.GetComponent( TRANSFORM_COMP );
		PhysicsComp* physics_comp = (PhysicsComp*)entity.GetComponent( PHYSICS_COMP );
		QuestGiverComp* quest_giver_comp = (QuestGiverComp*)entity.GetComponent( QUEST_GIVER_COMP );
		RenderComp* render_comp = (RenderComp*)entity.GetComponent( RENDER_COMP );

		if( interact_comp && trans_comp && physics_comp && quest_giver_comp && render_comp )
		{
			float radius = physics_comp->m_radius;
			// If a quest giver, put ! above head.
			AddVertsForAABB2D( render_comp->m_verts_groups["Data/Images/Extras_4x4.png"].verts, AABB2( radius, radius, trans_comp->m_transform.m_position + Vec2::UP * .35f ), Rgba::CYAN, Vec2( 0.75f, 0.0f ), Vec2( 1.0f, 0.25f ) );
			
			for (auto other_entity : GetCurrentAdmin().m_entities)
			{
				if( !other_entity.m_claimed || other_entity.m_id == entity.m_id )
				{
					// Don't check on self
					continue;
				}
				// Is entity the player?
				
				// Other entities comp
				QuestCarrierComp* other_player_carrier_comp = (QuestCarrierComp*)other_entity.GetComponent( QUEST_CARRIER_COMP );
				IntentComp* other_player_intent_comp = (IntentComp*)other_entity.GetComponent( INTENT_COMP );
				TransformComp* other_trans_comp = (TransformComp*)other_entity.GetComponent( TRANSFORM_COMP );
				CameraComp* other_camera_comp = (CameraComp*)other_entity.GetComponent( CAMERA_COMP );
				if( other_player_carrier_comp && other_trans_comp && other_player_intent_comp )
				{
					// Get displacement to player and check if the length squared is within range of the range squared
					Vec2 displcement = other_trans_comp->m_transform.m_position - trans_comp->m_transform.m_position;
					bool in_range = displcement.GetLengthSquared() < interact_comp->m_interact_range * interact_comp->m_interact_range;
					bool camera_active = other_camera_comp && other_camera_comp->m_active;

					if( in_range )
					{
						// Is in range of player
						if( other_player_intent_comp->m_wants_to_interact )
						{
							quest_giver_comp->triggered = true;
							if( quest_giver_comp->complete )
							{
								if ( other_player_carrier_comp->HasQuest(quest_giver_comp) )
								{
									TriggerQuestComplete( quest_giver_comp );
									other_player_carrier_comp->RemoveQuest( quest_giver_comp );
								}
							}
							else
							{
								if( !other_player_carrier_comp->HasQuest( quest_giver_comp ) )
								{
									other_player_carrier_comp->AddQuest( quest_giver_comp );
									TriggerQuestAccepted( quest_giver_comp );
								}
							}
						}
						if( render_comp )
						{
							BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelFixedFont" );
							render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
							font->AddVertsFor2DText( render_comp->m_verts_groups["SquirrelFixedFont"].verts, Vec2( trans_comp->m_transform.m_position ) - Vec2( radius * .5f, radius * 4.0f ) * .5f, .25f, "F", 0.5f, Rgba( 1.0f, .9f, .6f ) );
						}
					}
					else
					{
						quest_giver_comp->triggered = false;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
/**
* TriggerQuestComplete
*/
void QuestSystem::TriggerQuestComplete( QuestGiverComp* giver ) const
{
	// #TODO: Trigger Quest Complete logic 
}

//--------------------------------------------------------------------------
/**
* TriggerQuestAccepted
*/
void QuestSystem::TriggerQuestAccepted( QuestGiverComp* giver ) const
{
	// #TODO: Trigger Quest Accepted logic 
}
