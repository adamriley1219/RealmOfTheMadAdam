#include "Game/Components/Component.hpp"
//--------------------------------------------------------------------------
/**
* Component
*/
Component::Component()
{

}

//--------------------------------------------------------------------------
/**
* ~Component
*/
Component::~Component()
{

}

//--------------------------------------------------------------------------
/**
* GetClaimed
*/
void Component::GetClaimed( EntityID owner )
{
	m_owner = owner;
}
