#include "Game/EntityAdmin.hpp"
#include "Game/Components/Component.hpp"

#include "Game/Systems/CombatSystem.hpp"
#include "Game/Systems/GameInputSystem.hpp"
#include "Game/Systems/GamePhysicsSystem.hpp"
#include "Game/Systems/QuestSystem.hpp"
#include "Game/Systems/TriggerSystem.hpp"
#include "Game/Systems/UISystem.hpp"
#include "Game/Systems/RenderSystem.hpp"
#include "Game/Systems/MovementSystem.hpp"

#include "Game/Components/InputComp.hpp"
#include "Game/Components/NameComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/UIComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/AIComp.hpp"

#include "Game/Entity.hpp"

std::vector<System*> EntityAdmin::m_systems;
EntityAdmin EntityAdmin::m_master;

//--------------------------------------------------------------------------
/**
* Entity
*/
EntityAdmin::EntityAdmin()
{
	m_input_comps = new InputComp[MAX_ENTITIES];
	m_name_comps = new NameComp[MAX_ENTITIES];
	m_physics_comps = new PhysicsComp[MAX_ENTITIES];
	m_render_comps = new RenderComp[MAX_ENTITIES];
	m_Transform_comps = new TransformComp[MAX_ENTITIES];
	m_UI_comps = new UIComp[MAX_ENTITIES];
	m_quest_giver_comps = new QuestGiverComp[MAX_ENTITIES];
	m_quest_carrier_comps = new QuestCarrierComp[MAX_ENTITIES];
	m_intent_comps = new IntentComp[MAX_ENTITIES];
	m_ai_comps = new AIComp[MAX_ENTITIES];
	m_camera_comps = new CameraComp[MAX_ENTITIES];
	m_interact_comps = new InteractComp[MAX_ENTITIES];
}

//--------------------------------------------------------------------------
/**
* ~Entity
*/
EntityAdmin::~EntityAdmin()
{
	delete[] m_interact_comps;
	delete[] m_input_comps;
	delete[] m_name_comps;
	delete[] m_physics_comps;
	delete[] m_render_comps;
	delete[] m_Transform_comps;
	delete[] m_UI_comps;
	delete[] m_quest_giver_comps;
	delete[] m_quest_carrier_comps;
	delete[] m_intent_comps;
	delete[] m_ai_comps;
	delete[] m_camera_comps;
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
* GetComponent
*/
Component* EntityAdmin::GetComponent( EntityID id, eComponentType type )
{
	Entity& entity = m_entities[id];
	
	if( entity.m_claimed )
	{
		if( entity.HasComponent( type ) )
		{
			switch (type)
			{
			case INPUT_COMP:
				return &m_input_comps[id];
				break;
			case NAME_COMP:
				return &m_name_comps[id];
				break;
			case PHYSICS_COMP:
				return &m_physics_comps[id];
				break;
			case RENDER_COMP:
				return &m_render_comps[id];
				break;
			case TRANSFORM_COMP:
				return &m_Transform_comps[id];
				break;
			case UI_COMP:
				return &m_UI_comps[id];
				break;
			case QUEST_GIVER_COMP:
				return &m_quest_giver_comps[id];
				break;
			case QUEST_CARRIER_COMP:
				return &m_quest_carrier_comps[id];
				break;
			case INTENT_COMP:
				return &m_intent_comps[id];
				break;
			case AI_COMP:
				return &m_ai_comps[id];
				break;
			case CAMERA_COMP:
				return &m_camera_comps[id];
				break;
			case INTERACT_COMP:
				return &m_interact_comps[id];
				break;
			default:
				break;
			}
		}
	}
	return nullptr;
	
}

//--------------------------------------------------------------------------
/**
* CreateEntity
*/
Entity* EntityAdmin::CreateEntity()
{
	for( uint ent_idx = 0; ent_idx < MAX_ENTITIES; ++ent_idx )
	{
		Entity& entity = m_entities[ent_idx];
		if( !entity.m_claimed )
		{
			entity.m_id = ent_idx;
			entity.m_claimed = true;
			entity.m_owner = this;
			return &entity;
		}
	}
	ERROR_AND_DIE( "Not enough space within the admin to create another entity." );
}

//--------------------------------------------------------------------------
/**
* AddEntity
*/
Entity* EntityAdmin::AddEntity( const Entity& entity )
{
	Entity& new_entity = *CreateEntity();
	
	ASSERT_OR_DIE( entity.m_owner, "entity to add never blelonged to an admin." );
	for( uint idx = 0; idx < NUM_COMP_TYPES; --idx )
	{
		if( entity.HasComponent( (eComponentType)idx ) )
		{
			Component* comp_to_copy = entity.m_owner->GetComponent( entity.m_id, (eComponentType)idx );
			new_entity.CopyComponent( comp_to_copy );
		}
	}

	return &new_entity;
}

//--------------------------------------------------------------------------
/**
* RemoveEntity
*/
void EntityAdmin::RemoveEntity( EntityID id )
{
	m_entities[id].m_claimed = false;
}

//--------------------------------------------------------------------------
/**
* RemoveComponent
*/
void EntityAdmin::RemoveComponent( EntityID id, eComponentType type )
{
	m_entities[id].RemoveComponent( type );
}

