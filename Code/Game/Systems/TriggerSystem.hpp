#pragma once
#include "Game/Systems/System.hpp"


class TriggerSystem : public System
{
public:
	TriggerSystem();
	~TriggerSystem();

public:
	void Update( float deltaTime ) const;

private:
//	void UpdateTrigger( TriggerComp* trigger_comp );

};
