#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"

//--------------------------------------------------------------------------
/**
* UIComp
*/
QuestCarrierComp::QuestCarrierComp()
	: Component()
{
	
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

//--------------------------------------------------------------------------
/**
* Copy
*/
void QuestCarrierComp::Copy(const Component* copyFrom)
{
	*this = *( (QuestCarrierComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType QuestCarrierComp::GetType()
{
	return QUEST_CARRIER_COMP;
}
