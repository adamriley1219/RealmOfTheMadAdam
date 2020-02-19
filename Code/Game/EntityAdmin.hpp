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
struct IntentComp;
struct AIComp;
struct CameraComp;
struct InteractComp;

struct RenderSystemTuple;
struct MovementTuple;
struct InputTuple;

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

public:
	static EntityAdmin m_master;
	static std::vector<System*> m_systems;

	InputComp* m_input_comps = nullptr;
	NameComp* m_name_comps = nullptr;
	PhysicsComp* m_physics_comps = nullptr;
	RenderComp* m_render_comps = nullptr;
	TransformComp* m_Transform_comps = nullptr;
	UIComp* m_UI_comps = nullptr;
	QuestGiverComp* m_quest_giver_comps = nullptr;
	QuestCarrierComp* m_quest_carrier_comps = nullptr;
	IntentComp* m_intent_comps = nullptr;
	AIComp* m_ai_comps = nullptr;
	CameraComp* m_camera_comps = nullptr;
	InteractComp* m_interact_comps = nullptr;

	Entity m_entities[MAX_ENTITIES];
};

