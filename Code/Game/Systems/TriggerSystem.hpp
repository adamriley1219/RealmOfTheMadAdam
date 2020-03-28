#pragma once
#include "Game/Systems/System.hpp"

class Entity;

class TriggerSystem : public System
{
public:
	TriggerSystem();
	~TriggerSystem();

public:
	void Update( float deltaTime ) const;

private:
	void UpdateTriggerOnEntity( Entity& trigger, Entity& entity ) const;

};
