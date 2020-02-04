#include "Game/Systems/QuestSystem.hpp"
#include "Game/GameCommon.hpp"

#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/InputComp.hpp"
#include "Game/Components/IntentComp.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"


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
	for (auto ent_pair : GetCurrentAdmin().m_entities)
	{
		Entity& entity = *(ent_pair.second);

		InteractComp* interact_comp = (InteractComp*)entity.GetComponent( INTERACT_COMP );
		QuestGiverComp* quest_giver_comp = (QuestGiverComp*)entity.GetComponent( QUEST_GIVER_COMP );
		TransformComp* trans_comp = (TransformComp*)entity.GetComponent( TRANSFORM_COMP );
		if( interact_comp && trans_comp )
		{
			for (auto ent_other_pair : GetCurrentAdmin().m_entities)
			{
				// Is entity the player?
				Entity& other = *(ent_other_pair.second);
				QuestCarrierComp* player_carrier_comp = (QuestCarrierComp*)other.GetComponent( QUEST_CARRIER_COMP );
				IntentComp* player_intent_comp = (IntentComp*)other.GetComponent( INTENT_COMP );
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
					}
				}
			}
		}
	}
}
