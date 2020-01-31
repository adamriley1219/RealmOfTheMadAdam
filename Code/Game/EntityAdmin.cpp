#include "Game/EntityAdmin.hpp"
#include "Game/Components/Component.hpp"
#include "Game/Systems/RenderSystem.hpp"
#include "Game/Entity.hpp"

std::vector<System*> EntityAdmin::m_systems;

//--------------------------------------------------------------------------
/**
* Entity
*/
EntityAdmin::EntityAdmin()
{

}

//--------------------------------------------------------------------------
/**
* ~Entity
*/
EntityAdmin::~EntityAdmin()
{

}

//--------------------------------------------------------------------------
/**
* Render
*/
void EntityAdmin::Render() const
{
	for( System* system : m_systems )
	{
		system->Render();
	}
}

//--------------------------------------------------------------------------
/**
* Update
*/
void EntityAdmin::Update( float deltaTime )
{
	for (System* system : m_systems)
	{
		system->Update( deltaTime );
	}
}

//--------------------------------------------------------------------------
/**
* CreateEntity
*/
Entity* EntityAdmin::CreateEntity()
{
	Entity* entity = new Entity();
	if( m_entities.find( entity->m_id ) != m_entities.end() )
	{
		ERROR_RECOVERABLE( "Entity already exists in the map - EntityAdmin::CreateEntity" );
	}
	AddEntity( entity );
	return entity;
}

//--------------------------------------------------------------------------
/**
* AddEntity
*/
void EntityAdmin::AddEntity( Entity* entity )
{
	if( m_entities.find( entity->m_id ) != m_entities.end() )
	{
		ERROR_RECOVERABLE( "Adding Entity %U and it already exists in the map" );
	}
	m_entities[entity->m_id] = entity;
	entity->m_owner = this;
	
	for( Component* component : entity->m_components )
	{
		AddComponent( component );
	}
}

//--------------------------------------------------------------------------
/**
* AddComponent
*/
void EntityAdmin::AddComponent( Component* component )
{
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
* DeleteEntity
*/
void EntityAdmin::DeleteEntity( Entity* entity )
{
	RemoveEntity( entity );
	for( Component*& comp : entity->m_components )
	{
		SAFE_DELETE( comp );
	}
	SAFE_DELETE( entity );
}

//--------------------------------------------------------------------------
/**
* RemoveEntity
*/
void EntityAdmin::RemoveEntity( Entity* entity )
{
	if( m_entities.find( entity->m_id ) == m_entities.end() )
	{
		ERROR_RECOVERABLE( "Trying to remove entity that doesnt exist in the admin" );
	}
	for( Component* comp : entity->m_components )
	{
		RemoveComponent( comp );
	}
	m_entities.erase( entity->m_id );	
}

//--------------------------------------------------------------------------
/**
* RemoveComponent
*/
void EntityAdmin::RemoveComponent(Component* component)
{
	for( Component*& comp : m_components )
	{
		if( comp == component )
		{
			comp = nullptr;
			return;
		}
	}
}

//--------------------------------------------------------------------------
/**
* GetRenderTuples
*/
RenderSystemTuple* EntityAdmin::GetRenderTuple( EntityID entity_id ) const
{
	auto ent_itr = m_entities.find(entity_id);

	if( ent_itr == m_entities.end() )
	{
		return nullptr;
	}

	Entity* entity = ent_itr->second;

	TransformComp* trans_comp = (TransformComp*)entity->GetComponent( TRANSFORM_COMP );
	RenderComp* render_comp = (RenderComp*)entity->GetComponent( RENDER_COMP );

	if( trans_comp && render_comp )
	{
		RenderSystemTuple* tup = new RenderSystemTuple();
		tup->rendercomp = render_comp;
		tup->transformcomp = trans_comp;
		return tup;
	}

	return nullptr;
}
