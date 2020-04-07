#pragma once
#include "Game/Systems/System.hpp"

#include "Game/Components/StatsComp.hpp"

#include <string>

struct Transform2D;
struct Vec2;

class CombatSystem : public System
{
public:
	CombatSystem();
	~CombatSystem();

	void Update( float delta_time ) const;

private:
	void Fire( const std::string& ability_name, const Vec2& direction, float damage_multiplier, const Transform2D& starting_position, eTeam team ) const;
};
