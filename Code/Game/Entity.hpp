#pragma once
#include "Engine/Math/Vec2.hpp"

#include "Game/GameCommon.hpp"
#include "Game/Components/Component.hpp"

class EntityAdmin;
struct Component;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	Component* AddComponent( eComponentType type );
	void RemoveComponent( eComponentType type );

	bool AddComponent( Component* component );
	bool CopyComponent( Component* component );

	Component* GetComponent( eComponentType type ) const;

	bool HasComponent( eComponentType type ) const;

	int64_t m_bit_map_components = 0;
	
public:
	EntityID m_id = -1;
	bool m_claimed = false;
	bool m_destroy = false;

	int m_map_to_transfer_to = -1;

	static Entity s_protoEntity;

	EntityAdmin* m_owner = nullptr;

};

