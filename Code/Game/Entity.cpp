#include "Game/Entity.hpp"
#include "Game/Components/Component.hpp"
#include "Game/EntityAdmin.hpp"

//--------------------------------------------------------------------------
/**
* Entity
*/
Entity::Entity()
{

}

//--------------------------------------------------------------------------
/**
* ~Entity
*/
Entity::~Entity()
{

}


//--------------------------------------------------------------------------
/**
* AddComponent
*/
Component* Entity::AddComponent( eComponentType type )
{
	SetBitFlags( m_bit_map_components, (size_t) 1 << type );
	return GetComponent( type );
}

//--------------------------------------------------------------------------
/**
* AddComponent
*/
bool Entity::AddComponent( Component* component )
{
	AddComponent( component->GetType() );
	return CopyComponent( component );
}

//--------------------------------------------------------------------------
/**
* RemoveComponent
*/
void Entity::RemoveComponent( eComponentType type )
{
	size_t type_flag = (size_t) 1 << type;
	ClearBitFlag( m_bit_map_components, type_flag );
}

//--------------------------------------------------------------------------
/**
* CopyComponent
*/
bool Entity::CopyComponent( Component* component )
{
	Component* owned_comp = m_owner->GetComponent( m_id, component->GetType() );
	if( owned_comp )
	{
		owned_comp->Copy(component);
		owned_comp->GetClaimed( m_id );
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
/**
* GetComponent
*/
Component* Entity::GetComponent( eComponentType type ) const
{
	size_t flags = (size_t) 1 << type;
	if( IsBitFlagSet( m_bit_map_components, flags ) )
	{
		return m_owner->GetComponent( m_id, type );
	}
	return nullptr;
}

//--------------------------------------------------------------------------
/**
* HasComponent
*/
bool Entity::HasComponent( eComponentType type ) const
{
	return IsBitFlagSet( m_bit_map_components, (size_t) 1 << type );
}
