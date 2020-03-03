#pragma once
#include "Game/Components/Component.hpp"


struct AIComp : public Component
{
public:
	AIComp();
	~AIComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

	float m_trigger_range = 0.0f;

private:
	static AIComp s_protoAIComp;
};
