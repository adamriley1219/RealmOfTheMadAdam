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
	bool	m_isTrigger = false;
	bool	m_die_on_contect = false;

	float	m_damage_on_triggered = 0.0f;

private:
	static TriggerComp s_prototype;

};
