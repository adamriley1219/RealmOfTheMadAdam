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
#include "Game/Components/QuestComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/AIComp.hpp"
#include "Game/Components/AbilityComp.hpp"
#include "Game/Components/TriggerComp.hpp"
#include "Game/Components/StatsComp.hpp"

#include "Game/Entity.hpp"

std::vector<System*> EntityAdmin::m_systems;

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
	m_quest_comps = new QuestComp[MAX_ENTITIES];
	m_intent_comps = new IntentComp[MAX_ENTITIES];
	m_ai_comps = new AIComp[MAX_ENTITIES];
	m_camera_comps = new CameraComp[MAX_ENTITIES];
	m_interact_comps = new InteractComp[MAX_ENTITIES];
	m_ability_comps = new AbilityComp[MAX_ENTITIES];
	m_trigger_comps = new TriggerComp[MAX_ENTITIES];
	m_stats_comps = new StatsComp[MAX_ENTITIES];

	m_list_of_input_comps.resize(MAX_ENTITIES);
	m_list_of_name_comps.resize(MAX_ENTITIES);
	m_list_of_physics_comps.resize(MAX_ENTITIES);
	m_list_of_render_comps.resize(MAX_ENTITIES);
	m_list_of_transform_comps.resize(MAX_ENTITIES);
	m_list_of_UI_comps.resize(MAX_ENTITIES);
	m_list_of_quest_giver_comps.resize(MAX_ENTITIES);
	m_list_of_quest_carrier_comps.resize(MAX_ENTITIES);
	m_list_of_quest_comps.resize(MAX_ENTITIES);
	m_list_of_intent_comps.resize(MAX_ENTITIES);
	m_list_of_AI_comps.resize(MAX_ENTITIES);
	m_list_of_camera_comps.resize(MAX_ENTITIES);
	m_list_of_interact_comps.resize(MAX_ENTITIES);
	m_list_of_ability_comps.resize(MAX_ENTITIES);
	m_list_of_trigger_comps.resize(MAX_ENTITIES);
	m_list_of_stat_comps.resize(MAX_ENTITIES);
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
	delete[] m_quest_comps;
	delete[] m_intent_comps;
	delete[] m_ai_comps;
	delete[] m_camera_comps;
	delete[] m_ability_comps;
	delete[] m_trigger_comps;
	delete[] m_stats_comps;
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
	UpdateStateOfCompLists();

	for( System* system : m_systems )
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
			case QUEST_COMP:
				return &m_quest_comps[id];
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
			case ABILITY_COMP:
				return &m_ability_comps[id];
				break;
			case TRIGGER_COMP:
				return &m_trigger_comps[id];
				break;
			case STATS_COMP:
				return &m_stats_comps[id];
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
			entity = Entity::s_protoEntity;
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
	
	ASSERT_OR_DIE( entity.m_owner, "entity to add never belonged to an admin." );

	new_entity.m_bit_map_components = entity.m_bit_map_components;

	for( uint idx = 0; idx < NUM_COMP_TYPES; ++idx )
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

//--------------------------------------------------------------------------
/**
* GetNumEntites
*/
uint EntityAdmin::GetNumEntites() const
{
	uint count = 0;
	for( const Entity& entity : m_entities )
	{
		if( entity.m_claimed )
		{
			++count;
		}
	}
	return count;
}

//--------------------------------------------------------------------------
/**
* GetFirstWithComp
*/
Entity* EntityAdmin::GetFirstWithComp( eComponentType type )
{
	switch (type)
	{
	case INPUT_COMP:
		if( m_list_of_input_comps.size() > 0 )
		{
			return m_list_of_input_comps[0];
		}
		break;
	case NAME_COMP:
		if( m_list_of_name_comps.size() > 0 )
		{
			return m_list_of_name_comps[0];
		}
		break;
	case PHYSICS_COMP:
		if( m_list_of_physics_comps.size() > 0 )
		{
			return m_list_of_physics_comps[0];
		}
		break;
	case RENDER_COMP:
		if( m_list_of_render_comps.size() > 0 )
		{
			return m_list_of_render_comps[0];
		}
		break;
	case TRANSFORM_COMP:
		if( m_list_of_transform_comps.size() > 0 )
		{
			return m_list_of_transform_comps[0];
		}
		break;
	case UI_COMP:
		if( m_list_of_UI_comps.size() > 0 )
		{
			return m_list_of_UI_comps[0];
		}
		break;
	case QUEST_GIVER_COMP:
		if( m_list_of_quest_giver_comps.size() > 0 )
		{
			return m_list_of_quest_giver_comps[0];
		}
		break;
	case QUEST_CARRIER_COMP:
		if( m_list_of_quest_carrier_comps.size() > 0 )
		{
			return m_list_of_quest_carrier_comps[0];
		}
		break;
	case QUEST_COMP:
		if( m_list_of_quest_comps.size() > 0 )
		{
			return m_list_of_quest_comps[0];
		}
		break;
	case INTENT_COMP:
		if( m_list_of_intent_comps.size() > 0 )
		{
			return m_list_of_intent_comps[0];
		}
		break;
	case AI_COMP:
		if( m_list_of_AI_comps.size() > 0 )
		{
			return m_list_of_AI_comps[0];
		}
		break;
	case CAMERA_COMP:
		if( m_list_of_camera_comps.size() > 0 )
		{
			return m_list_of_camera_comps[0];
		}
		break;
	case INTERACT_COMP:
		if( m_list_of_interact_comps.size() > 0 )
		{
			return m_list_of_interact_comps[0];
		}
		break;
	case ABILITY_COMP:
		if( m_list_of_ability_comps.size() > 0 )
		{
			return m_list_of_ability_comps[0];
		}
		break;
	case TRIGGER_COMP:
		if( m_list_of_trigger_comps.size() > 0 )
		{
			return m_list_of_trigger_comps[0];
		}
		break;
	case STATS_COMP:
		if( m_list_of_stat_comps.size() > 0 )
		{
			return m_list_of_stat_comps[0];
		}
		break;
	default:
		break;
	}
	return nullptr;
}

//--------------------------------------------------------------------------
/**
* GetAllWithComp
*/
std::vector<Entity*>& EntityAdmin::GetAllWithComp( eComponentType type )
{
	switch (type)
	{
	case INPUT_COMP:
		return m_list_of_input_comps;
		break;
	case NAME_COMP:
		return m_list_of_name_comps;
		break;
	case PHYSICS_COMP:
		return m_list_of_physics_comps;
		break;
	case RENDER_COMP:
		return m_list_of_render_comps;
		break;
	case TRANSFORM_COMP:
		return m_list_of_transform_comps;
		break;
	case UI_COMP:
		return m_list_of_UI_comps;
		break;
	case QUEST_GIVER_COMP:
		return m_list_of_quest_giver_comps;
		break;
	case QUEST_CARRIER_COMP:
		return m_list_of_quest_carrier_comps;
		break;
	case QUEST_COMP:
		return m_list_of_quest_comps;
		break;
	case INTENT_COMP:
		return m_list_of_intent_comps;
		break;
	case AI_COMP:
		return m_list_of_AI_comps;
		break;
	case CAMERA_COMP:
		return m_list_of_camera_comps;
		break;
	case INTERACT_COMP:
		return m_list_of_interact_comps;
		break;
	case ABILITY_COMP:
		return m_list_of_ability_comps;
		break;
	case TRIGGER_COMP:
		return m_list_of_trigger_comps;
		break;
	case STATS_COMP:
		return m_list_of_stat_comps;
		break;
	default:
		break;
	}
	static std::vector<Entity*> empty_list;
	return empty_list;
}

//--------------------------------------------------------------------------
/**
* UpdateStateOfCompLists
*/
void EntityAdmin::UpdateStateOfCompLists()
{
	m_list_of_input_comps.clear();
	m_list_of_name_comps.clear();
	m_list_of_physics_comps.clear();
	m_list_of_render_comps.clear();
	m_list_of_transform_comps.clear();
	m_list_of_UI_comps.clear();
	m_list_of_quest_giver_comps.clear();
	m_list_of_quest_carrier_comps.clear();
	m_list_of_quest_comps.clear();
	m_list_of_intent_comps.clear();
	m_list_of_AI_comps.clear();
	m_list_of_camera_comps.clear();
	m_list_of_interact_comps.clear();
	m_list_of_ability_comps.clear();
	m_list_of_trigger_comps.clear();
	m_list_of_stat_comps.clear();

	for( uint idx = 0; idx < MAX_ENTITIES; ++idx )
	{
		Entity& entity = m_entities[idx];
		
		if( entity.HasComponent( INPUT_COMP ) )
		{
			m_list_of_input_comps.push_back( &entity );
		}
		if( entity.HasComponent( NAME_COMP ) )
		{
			m_list_of_name_comps.push_back( &entity );
		}
		if( entity.HasComponent( PHYSICS_COMP ) )
		{
			m_list_of_physics_comps.push_back( &entity );
		}
		if( entity.HasComponent( RENDER_COMP ) )
		{
			m_list_of_render_comps.push_back( &entity );
		}
		if( entity.HasComponent( TRANSFORM_COMP ) )
		{
			m_list_of_transform_comps.push_back(&entity);
		}
		if( entity.HasComponent( UI_COMP ) )
		{
			m_list_of_UI_comps.push_back(&entity);
		}
		if( entity.HasComponent( QUEST_GIVER_COMP ) )
		{
			m_list_of_quest_giver_comps.push_back(&entity);
		}
		if( entity.HasComponent( QUEST_CARRIER_COMP ) )
		{
			m_list_of_quest_carrier_comps.push_back(&entity);
		}
		if( entity.HasComponent( QUEST_COMP ) )
		{
			m_list_of_quest_comps.push_back(&entity);
		}
		if( entity.HasComponent( AI_COMP ) )
		{
			m_list_of_AI_comps.push_back(&entity);
		}
		if( entity.HasComponent( INTENT_COMP ) )
		{
			m_list_of_intent_comps.push_back(&entity);
		}
		if( entity.HasComponent( CAMERA_COMP ) )
		{
			m_list_of_camera_comps.push_back(&entity);
		}
		if( entity.HasComponent( INTERACT_COMP ) )
		{
			m_list_of_interact_comps.push_back(&entity);
		}
		if( entity.HasComponent( ABILITY_COMP ) )
		{
			m_list_of_ability_comps.push_back(&entity);
		}
		if( entity.HasComponent( TRIGGER_COMP ) )
		{
			m_list_of_trigger_comps.push_back(&entity);
		}
		if( entity.HasComponent( STATS_COMP ) )
		{
			m_list_of_stat_comps.push_back(&entity);
		}
	}
}

//--------------------------------------------------------------------------
/**
* GetMaster
*/
EntityAdmin* EntityAdmin::GetMaster()
{
	static EntityAdmin* master = new EntityAdmin();
	return master;
}

