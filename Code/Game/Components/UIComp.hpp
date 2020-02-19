#pragma once
#include "Game/Components/Component.hpp"



struct UIComp : public Component
{
public:
	UIComp();
	~UIComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();

private:

};
