#include "Game/Components/QuestGiverComp.hpp"


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
* GetKillEnemiesText
*/
std::string QuestGiverComp::GetKillEnemiesText() const
{
	return Stringf( "Kill enemies %u/%u", num_enemies_killed, num_enemies_to_kill );
}

//--------------------------------------------------------------------------
/**
* GetDialog
*/
std::string QuestGiverComp::GetDialog() const
{
	if( complete )
	{
		return complete_text;
	}
	return init_text;
}
