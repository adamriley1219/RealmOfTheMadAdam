#include "Game/Components/IntentComp.hpp"


//--------------------------------------------------------------------------
/**
* UIComp
*/
IntentComp::IntentComp()
	: Component()
{
	
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
