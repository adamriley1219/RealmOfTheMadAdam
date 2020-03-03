#pragma once
#include "Game/Components/Component.hpp"


struct AbilityComp : public Component
{
public:
	AbilityComp();
	~AbilityComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

private:
	static AbilityComp s_protoAblityComp;
};
