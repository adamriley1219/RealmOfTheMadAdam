#include "Game/Components/PhysicsComp.hpp"


PhysicsComp PhysicsComp::s_prototype;

//--------------------------------------------------------------------------
/**
* PhysicsComp
*/
PhysicsComp::PhysicsComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* PhysicsComp
*/
PhysicsComp::PhysicsComp(bool static_object)
	: Component(), m_static_object( static_object )
{

}

//--------------------------------------------------------------------------
/**
* ~PhysicsComp
*/
PhysicsComp::~PhysicsComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void PhysicsComp::Copy(const Component* copyFrom)
{
	*this = *( (PhysicsComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType PhysicsComp::GetType()
{
	return PHYSICS_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void PhysicsComp::Reset()
{
	*this = s_prototype;
}
