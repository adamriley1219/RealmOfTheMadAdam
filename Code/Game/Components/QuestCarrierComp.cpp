#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"

//--------------------------------------------------------------------------
/**
* UIComp
*/
QuestCarrierComp::QuestCarrierComp()
	: Component()
{
	m_type = QUEST_CARRIER_COMP;
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
QuestCarrierComp::~QuestCarrierComp()
{

}

//--------------------------------------------------------------------------
/**
* AddQuest
*/
void QuestCarrierComp::AddQuest( QuestGiverComp* quest_giver )
{
	quest_givers.push_back( quest_giver );
}
