#include "Game/Entity.hpp"
#include "Game/Components/Component.hpp"
#include "Game/EntityAdmin.hpp"

//--------------------------------------------------------------------------
/**
* Entity
*/
Entity::Entity()
{
	m_id = GetNextID();
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
void Entity::AddComponent( Component* component )
{
	component->GetClaimed( m_id );
	m_owner->AddComponent( component );
	for( Component*& comp : m_components )
	{
		if( !comp )
		{
			comp = component;
			return;
		}
	}
	m_components.push_back( component );
}

//--------------------------------------------------------------------------
/**
* RemoveComponent
*/
void Entity::RemoveComponent(Component* component)
{
	m_owner->RemoveComponent( component );
	for( Component*& comp : m_components )
	{
		if( comp == component )
		{
			component->GetClaimed( 0 );
			comp = nullptr;
			return;
		}
	}
}

//--------------------------------------------------------------------------
/**
* GetComponent
*/
Component* Entity::GetComponent( eComponentType type )
{
	for( Component* comp : m_components )
	{
		if( comp->m_type == type )
		{
			return comp;
		}
	}
	return nullptr;
}

//--------------------------------------------------------------------------
/**
* GetNextID
*/
EntityID Entity::GetNextID()
{
	static EntityID entity_id_counter = 1;
	if( entity_id_counter == 0 )
	{
		++entity_id_counter;
	}
	return entity_id_counter++;
}
