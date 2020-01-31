#pragma once
#include "Game/Systems/System.hpp"

struct PhysicsComp;
struct TransformComp;

struct MovementTuple
{
	TransformComp* trans_comp;
	PhysicsComp* phys_comp;
};

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem();

	void Update( float deltaTime ) const;

private:

};
