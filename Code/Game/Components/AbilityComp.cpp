#include "Game/Components/AbilityComp.hpp"


//--------------------------------------------------------------------------
/**
* UIComp
*/
AbilityComp::AbilityComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
AbilityComp::~AbilityComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void AbilityComp::Copy(const Component* copyFrom)
{
	*this = *( (AbilityComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType AbilityComp::GetType()
{
	return ABILITY_COMP;
}
