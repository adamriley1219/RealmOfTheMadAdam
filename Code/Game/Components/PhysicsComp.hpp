#pragma once
#include "Game/Components/Component.hpp"


struct PhysicsComp : public Component
{
public:
	PhysicsComp();
	~PhysicsComp();

public:
	float m_speed = 1;

};
