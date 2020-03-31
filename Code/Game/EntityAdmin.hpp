#pragma once
#include <vector>
#include <unordered_map>

#include "Game/GameCommon.hpp"
#include "Game/Entity.hpp"

class System;

struct Component;
struct InputComp;
struct NameComp;
struct PhysicsComp;
struct RenderComp;
struct TransformComp;
struct UIComp;
struct QuestGiverComp;
struct QuestCarrierComp;
struct QuestComp;
struct IntentComp;
struct AIComp;
struct CameraComp;
struct InteractComp;
struct AbilityComp;
struct TriggerComp;
struct StatsComp;


class EntityAdmin
{
public:
	EntityAdmin();
	virtual ~EntityAdmin();

	void Render() const;
	void Update( float deltaTime );

public:
	Component* GetComponent( EntityID id, eComponentType type );

public:
	Entity* CreateEntity();

	// Adds an entity that's a copy of the entity passed in.
	Entity* AddEntity( const Entity& entity );

	void RemoveEntity( EntityID entity );
	void RemoveComponent( EntityID id, eComponentType type );

	Entity* GetEntity( EntityID id );

	uint GetNumEntites() const;

	Entity* GetFirstWithComp( eComponentType type );
	std::vector<Entity*>& GetAllWithComp( eComponentType type );

private:
	void UpdateStateOfCompLists();

public:
	static EntityAdmin* GetMaster();
	static std::vector<System*> m_systems;

	InputComp* m_input_comps = nullptr;
	NameComp* m_name_comps = nullptr;
	PhysicsComp* m_physics_comps = nullptr;
	RenderComp* m_render_comps = nullptr;
	TransformComp* m_Transform_comps = nullptr;
	UIComp* m_UI_comps = nullptr;
	QuestGiverComp* m_quest_giver_comps = nullptr;
	QuestCarrierComp* m_quest_carrier_comps = nullptr;
	QuestComp* m_quest_comps = nullptr;
	IntentComp* m_intent_comps = nullptr;
	AIComp* m_ai_comps = nullptr;
	CameraComp* m_camera_comps = nullptr;
	InteractComp* m_interact_comps = nullptr;
	AbilityComp* m_ability_comps = nullptr;
	TriggerComp* m_trigger_comps = nullptr;
	StatsComp* m_stats_comps = nullptr;

	Entity m_entities[MAX_ENTITIES];

private:
	std::vector<Entity*> m_list_of_input_comps;
	std::vector<Entity*> m_list_of_name_comps;
	std::vector<Entity*> m_list_of_physics_comps;
	std::vector<Entity*> m_list_of_render_comps;
	std::vector<Entity*> m_list_of_transform_comps;
	std::vector<Entity*> m_list_of_UI_comps;
	std::vector<Entity*> m_list_of_quest_giver_comps;
	std::vector<Entity*> m_list_of_quest_carrier_comps;
	std::vector<Entity*> m_list_of_quest_comps;
	std::vector<Entity*> m_list_of_intent_comps;
	std::vector<Entity*> m_list_of_AI_comps;
	std::vector<Entity*> m_list_of_camera_comps;
	std::vector<Entity*> m_list_of_interact_comps;
	std::vector<Entity*> m_list_of_ability_comps;
	std::vector<Entity*> m_list_of_trigger_comps;
	std::vector<Entity*> m_list_of_stat_comps;
	
};

