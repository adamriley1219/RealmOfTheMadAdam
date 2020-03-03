#pragma once
#include "Game/Components/Component.hpp"



struct InteractComp : public Component
{
public:
	InteractComp();
	InteractComp( float range );
	~InteractComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	float m_interact_range = 1.0f;

private:
	static InteractComp s_prototype;
};
