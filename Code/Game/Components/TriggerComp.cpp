#include "Game/Components/TriggerComp.hpp"


TriggerComp TriggerComp::s_prototype;

//--------------------------------------------------------------------------
/**
* UIComp
*/
TriggerComp::TriggerComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
TriggerComp::~TriggerComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void TriggerComp::Copy( const Component* copyFrom )
{
	*this = *( (TriggerComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType TriggerComp::GetType()
{
	return TRIGGER_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void TriggerComp::Reset()
{
	*this = s_prototype;
}
