#include "Game/Components/IntentComp.hpp"
#include "Game/App.hpp"

//--------------------------------------------------------------------------
/**
* UIComp
*/
IntentComp::IntentComp()
	: Component(),
	m_fire_timer( g_theApp->GetGameClock() ),
	m_death_timer( g_theApp->GetGameClock() )
{
	m_fire_timer.SetAndReset( 0.5f );
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
IntentComp::~IntentComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void IntentComp::Copy(const Component* copyFrom)
{
	*this = *( (IntentComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType IntentComp::GetType()
{
	return INTENT_COMP;
}
