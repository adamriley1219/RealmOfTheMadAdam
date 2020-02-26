#include "Game/Entity.hpp"
#include "Game/Components/Component.hpp"
#include "Game/EntityAdmin.hpp"



Entity Entity::s_protoEntity;



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
	SetBitFlags( m_bit_map_components, 1ll << type );
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
	int64_t type_flag = 1ll << type;
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
	int64_t flags = 1ll << type;
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
	return IsBitFlagSet( m_bit_map_components, 1ll << type );
}
