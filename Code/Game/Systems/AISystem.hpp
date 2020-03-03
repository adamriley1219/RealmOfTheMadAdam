#pragma once
#include "Game/Systems/System.hpp"

#include "Engine/Math/Vec2.hpp"

#include "Game/Components/StatsComp.hpp"

struct Entity;

class AISystem : public System
{
public:
	AISystem();
	~AISystem();

	virtual void Update( float deltaTime ) const;

private:
	Entity* GetClosetsEnemyInRange( const Vec2& position, float range, const StatsComp& to_check_stats ) const;

};


