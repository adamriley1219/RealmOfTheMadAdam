#include "Game/Systems/QuestSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/InputComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/RenderComp.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/BitmapFont.hpp"


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
		InteractComp* interact_comp = (InteractComp*)entity.GetComponent( INTERACT_COMP );
		QuestGiverComp* quest_giver_comp = (QuestGiverComp*)entity.GetComponent( QUEST_GIVER_COMP );
		TransformComp* trans_comp = (TransformComp*)entity.GetComponent( TRANSFORM_COMP );
		RenderComp* render_comp = (RenderComp*)entity.GetComponent( RENDER_COMP );
		if( interact_comp && trans_comp )
		{
			for (auto other_entity : GetCurrentAdmin().m_entities)
			{
				// Is entity the player?
				QuestCarrierComp* player_carrier_comp = (QuestCarrierComp*)other_entity.GetComponent( QUEST_CARRIER_COMP );
				IntentComp* player_intent_comp = (IntentComp*)other_entity.GetComponent( INTENT_COMP );
				TransformComp* trans_other_comp = (TransformComp*)entity.GetComponent(TRANSFORM_COMP);
				if( player_carrier_comp && trans_other_comp && player_intent_comp )
				{
					// Get displacement to player and check if the length squared is within range of the range squared
					Vec2 displcement = trans_other_comp->m_transform.m_position - trans_comp->m_transform.m_position;
					if( displcement.GetLengthSquared() < interact_comp->m_interact_range * interact_comp->m_interact_range )
					{
						// Is in range of player
						if( player_intent_comp->m_wants_to_interact )
						{
							player_carrier_comp->AddQuest( quest_giver_comp );
						}
						if( render_comp )
						{
							BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelFixedFont" );
							render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
							font->AddVertsFor2DText( render_comp->m_verts_groups["SquirrelFixedFont"].verts, Vec2( trans_comp->m_transform.m_position ), .75f, "F" );
						}
					}
				}
			}
		}
	}
}
