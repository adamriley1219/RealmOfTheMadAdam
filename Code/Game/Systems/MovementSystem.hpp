#pragma once
#include "Game/Systems/System.hpp"

struct PhysicsComp;
struct TransformComp;
struct IntentComp;

struct MovementTuple
{
	TransformComp* trans_comp;
	PhysicsComp* phys_comp;
	IntentComp* intent_comp;
};

class MovementSystem : public System
{
public:
	MovementSystem();
	~MovementSystem();

	void Update( float deltaTime ) const;

private:

};
