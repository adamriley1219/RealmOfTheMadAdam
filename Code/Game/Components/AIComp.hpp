#pragma once
#include "Game/Components/Component.hpp"



struct AIComp : public Component
{
public:
	AIComp();
	~AIComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();

private:

};
