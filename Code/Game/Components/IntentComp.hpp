#pragma once
#include "Game/Components/Component.hpp"



struct IntentComp : public Component
{
public:
	IntentComp();
	~IntentComp();

public:
	Vec2 m_desired_move_direction = Vec2::ZERO;
	bool m_wants_to_interact = false;
	bool m_wants_to_fire = false;
	bool m_wants_to_fire_secondary = false;
	
};
