#include "Game/Components/QuestGiverComp.hpp"


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
