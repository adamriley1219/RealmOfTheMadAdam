#pragma once
#include "Engine/Math/Vec2.hpp"

#include "Game/GameCommon.hpp"
#include "Game/Components/Component.hpp"

class EntityAdmin;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void AddComponent( Component* component );
	void RemoveComponent( Component* component );

	Component* GetComponent( eComponentType type );

	std::vector<Component*> m_components;

public:
	EntityID m_id = 0;
	EntityAdmin* m_owner = nullptr;

private:
	static EntityID GetNextID();
};

