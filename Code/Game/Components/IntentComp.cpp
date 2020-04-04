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

//--------------------------------------------------------------------------
/**
* Reset
*/
void IntentComp::Reset()
{
	m_desired_move_direction = Vec2::ZERO;
	m_wants_to_interact = false;
	m_has_ported = false;

	m_desired_fire_direction = Vec2::ZERO;
	m_wants_to_fire = false;
	m_wants_to_fire_secondary = false;
	m_fire_name = "UNINIT_FIRE_NAME";
	m_fire_secondary_name = "UNINIT_FIRE_SEC_NAME";
}
