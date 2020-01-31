#pragma once
#include "Game/Entity.hpp"

#include <vector>
#include <unordered_map>

class System;
class Entity;
class Component;
struct RenderSystemTuple;

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

	RenderSystemTuple* GetRenderTuple( EntityID entity_id ) const;

public:
	static std::vector<System*> m_systems;
	std::vector<Component*> m_components;
	std::unordered_map<EntityID, Entity*> m_entities; 
};

