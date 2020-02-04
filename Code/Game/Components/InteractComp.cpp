#include "Game/Components/InteractComp.hpp"


//--------------------------------------------------------------------------
/**
* UIComp
*/
InteractComp::InteractComp()
	: Component()
{
	m_type = INTERACT_COMP;
}

//--------------------------------------------------------------------------
/**
* InteractComp
*/
InteractComp::InteractComp( float range )
	: Component(), m_interact_range( range )
{
	m_type = INTERACT_COMP;
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
InteractComp::~InteractComp()
{

}
