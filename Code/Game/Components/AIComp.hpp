#pragma once
#include "Game/Components/Component.hpp"

enum eAttitude
{
	AGGRESSIVE_ATTITUDE,
	NUM_ATTITUDES
};


struct AIComp : public Component
{
public:
	AIComp();
	~AIComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

	float m_vision_radius = 0.0f;

private:
	static AIComp s_protoAIComp;
};
