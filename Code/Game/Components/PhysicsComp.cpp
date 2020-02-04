#include "Game/Components/PhysicsComp.hpp"


//--------------------------------------------------------------------------
/**
* PhysicsComp
*/
PhysicsComp::PhysicsComp()
	: Component()
{
	m_type = PHYSICS_COMP;
}

//--------------------------------------------------------------------------
/**
* PhysicsComp
*/
PhysicsComp::PhysicsComp(bool static_object)
	: Component(), m_static_object( static_object )
{
	m_type = PHYSICS_COMP;
}

//--------------------------------------------------------------------------
/**
* ~PhysicsComp
*/
PhysicsComp::~PhysicsComp()
{

}
