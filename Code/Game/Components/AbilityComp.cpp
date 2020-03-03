#include "Game/Components/AbilityComp.hpp"


AbilityComp AbilityComp::s_protoAblityComp;

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

//--------------------------------------------------------------------------
/**
* Reset
*/
void AbilityComp::Reset()
{
	*this = s_protoAblityComp;
}
