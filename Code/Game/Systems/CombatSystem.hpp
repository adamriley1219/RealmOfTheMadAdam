#pragma once
#include "Game/Systems/System.hpp"


class CombatSystem : public System
{
public:
	CombatSystem();
	~CombatSystem();

	void Update( float delta_time ) const;

private:

};
