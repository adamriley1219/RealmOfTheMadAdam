#include "Game/Components/InteractComp.hpp"


//--------------------------------------------------------------------------
/**
* UIComp
*/
InteractComp::InteractComp()
	: Component()
{

}

//--------------------------------------------------------------------------
/**
* InteractComp
*/
InteractComp::InteractComp( float range )
	: Component(), m_interact_range( range )
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
InteractComp::~InteractComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void InteractComp::Copy(const Component* copyFrom)
{
	*this = *( (InteractComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType InteractComp::GetType()
{
	return INTERACT_COMP;
}
