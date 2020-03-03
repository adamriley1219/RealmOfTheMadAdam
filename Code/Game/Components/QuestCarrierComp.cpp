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
void QuestCarrierComp::AddQuest( QuestGiverComp* quest_giver )
{
	for( QuestGiverComp*& giver : quest_givers )
	{
		if( !giver )
		{
			giver = quest_giver;
			return;
		}
	}
	quest_givers.push_back( quest_giver );
}

//--------------------------------------------------------------------------
/**
* RemoveQuest
*/
void QuestCarrierComp::RemoveQuest(QuestGiverComp* quest_giver)
{
	for( QuestGiverComp*& giver : quest_givers )
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
bool QuestCarrierComp::HasQuest( QuestGiverComp* quest_giver )
{
	for( auto quester : quest_givers )
	{
		if( quester && quester == quest_giver )
		{
			return true;
		}
	}
	return false;
}
