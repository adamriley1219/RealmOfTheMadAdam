#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestComp.hpp"

QuestGiverComp QuestGiverComp::s_prototype;

//--------------------------------------------------------------------------
/**
* UIComp
*/
QuestGiverComp::QuestGiverComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
QuestGiverComp::~QuestGiverComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void QuestGiverComp::Copy(const Component* copyFrom)
{
	*this = *( (QuestGiverComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType QuestGiverComp::GetType()
{
	return QUEST_GIVER_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void QuestGiverComp::Reset()
{
	*this = s_prototype;
}

//--------------------------------------------------------------------------
/**
* GetCurrentQuest
*/
QuestComp* QuestGiverComp::GetCurrentQuest() const
{
	for( QuestComp* quest : quests )
	{
		if( quest->is_active )
		{
			return quest;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------
/**
* AddQuest
*/
void QuestGiverComp::AddQuest( QuestComp* quest_to_add )
{
	quests.push_back( quest_to_add );
}

//--------------------------------------------------------------------------
/**
* RemoveQuest
*/
void QuestGiverComp::RemoveQuest( QuestComp* quest_to_remove )
{
	for( int idx = 0; idx < quests.size(); ++idx )
	{
		if( quests[idx] == quest_to_remove )
		{
			quests.erase( quests.begin() + idx );
			return;
		}
	}
}
