#pragma once
#include "Game/Systems/System.hpp"

struct PhysicsComp;
struct TransformComp;

struct PhysicsTuple
{
	PhysicsComp* physics_comp;
	TransformComp* trans_comp;
};

class GamePhysicsSystem : public System
{
public:
	GamePhysicsSystem();
	~GamePhysicsSystem();

	void Update( float deltaTime ) const;

private:

};
