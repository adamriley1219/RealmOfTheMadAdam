#include "Game/Components/QuestComp.hpp"


QuestComp QuestComp::s_prototype;

//--------------------------------------------------------------------------
/**
* UIComp
*/
QuestComp::QuestComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
QuestComp::~QuestComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void QuestComp::Copy(const Component* copyFrom)
{
	*this = *( (QuestComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType QuestComp::GetType()
{
	return QUEST_GIVER_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void QuestComp::Reset()
{
	*this = s_prototype;
}

//--------------------------------------------------------------------------
/**
* GetKillEnemiesText
*/
std::string QuestComp::GetKillEnemiesText() const
{
	return Stringf( "Kill enemies %u/%u", num_enemies_killed, num_enemies_to_kill );
}

//--------------------------------------------------------------------------
/**
* GetDialog
*/
std::string QuestComp::GetDialog() const
{
	if( state >= STATE_COMPLETE )
	{
		return complete_text;
	}
	return init_text;
}
