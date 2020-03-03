#include "Game/Components/StatsComp.hpp"


StatsComp StatsComp::s_prototype;

//--------------------------------------------------------------------------
/**
* UIComp
*/
StatsComp::StatsComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
StatsComp::~StatsComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void StatsComp::Copy( const Component* copyFrom )
{
	*this = *( (StatsComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType StatsComp::GetType()
{
	return STATS_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void StatsComp::Reset()
{
	*this = s_prototype;
}

//--------------------------------------------------------------------------
/**
* IsEnemy
*/
bool StatsComp::IsEnemy( eTeam team ) const
{
	return team != NUTRAL_TEAM ? m_team != team : false;
}
