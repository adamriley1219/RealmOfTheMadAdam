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

QuestCarrierComp QuestCarrierComp::s_prototype;

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
void QuestCarrierComp::AddQuest( QuestComp* quest_giver )
{
	for( QuestComp*& giver : quests )
	{
		if( !giver )
		{
			giver = quest_giver;
			return;
		}
	}
	quests.push_back( quest_giver );
}

//--------------------------------------------------------------------------
/**
* RemoveQuest
*/
void QuestCarrierComp::RemoveQuest( QuestComp* quest_giver )
{
	for( QuestComp*& giver : quests )
	{
		if( giver && giver == quest_giver )
		{
			giver = nullptr;
			return;
		}
	}
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

//--------------------------------------------------------------------------
/**
* Reset
*/
void QuestCarrierComp::Reset()
{
	*this = s_prototype;
}

//--------------------------------------------------------------------------
/**
* HasQuest
*/
bool QuestCarrierComp::HasQuest( QuestComp* quest )
{
	for( QuestComp* to_check_quest : quests )
	{
		if( to_check_quest && to_check_quest == quest )
		{
			return true;
		}
	}
	return false;
}
