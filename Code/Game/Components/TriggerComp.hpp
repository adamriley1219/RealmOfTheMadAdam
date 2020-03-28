#pragma once
#include "Game/Components/Component.hpp"



struct TriggerComp : public Component
{
public:
	TriggerComp();
	~TriggerComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	bool	m_die_on_contect = false;

	float	m_damage_on_triggered = 0.0f;

	bool	m_portal_active = false;
	Vec2	m_portal_to_position = Vec2::ZERO;

	bool	m_switch_main_texture = false;

	bool	m_transfer_map = false;
	int		m_map_to_transfer = -1;

private:
	static TriggerComp s_prototype;

};
