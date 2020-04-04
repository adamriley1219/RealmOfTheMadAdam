#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Core/Time/StopWatch.hpp"


struct IntentComp : public Component
{
public:
	IntentComp();
	~IntentComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	Vec2 m_desired_move_direction = Vec2::ZERO;
	bool m_wants_to_interact = false;
	bool m_has_ported = false;

	Vec2 m_desired_fire_direction = Vec2::ZERO;
	bool m_wants_to_fire = false;
	bool m_wants_to_fire_secondary = false;
	std::string m_fire_name = "UNINIT_FIRE_NAME";
	std::string m_fire_secondary_name = "UNINIT_FIRE_SEC_NAME";
	StopWatch m_fire_timer;

	StopWatch m_death_timer;

private:
	// cant do with timers static IntentComp s_prototype;
};
