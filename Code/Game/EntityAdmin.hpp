#pragma once
#include <vector>
#include <unordered_map>

#include "Game/GameCommon.hpp"

class System;
class Entity;

struct Component;
struct InputComp;

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
	Entity* CreateEntity();
	void AddEntity( Entity* entity );
	void AddComponent( Component* component );

	void DeleteEntity( Entity* entity );
	void RemoveEntity( Entity* entity );
	void RemoveComponent( Component* component );

public:
	RenderSystemTuple* GetRenderTuple( EntityID entity_id ) const;
	MovementTuple* GetMovementTuple( EntityID entity_id ) const;
	InputTuple* GetInputTuple( EntityID entity_id ) const;

public:
	static EntityAdmin m_master;
	static std::vector<System*> m_systems;
	std::vector<Component*> m_components;
	std::unordered_map<EntityID, Entity*> m_entities; 
};

