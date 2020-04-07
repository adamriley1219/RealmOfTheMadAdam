#pragma once
#include "Game/Components/Component.hpp"

enum eTeam
{
	ALLIED_TEAM,
	NUTRAL_TEAM,
	ENEMY_TEAM,
	NUM_TEAMS
};

struct StatsComp : public Component
{
public:
	StatsComp();
	~StatsComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

	bool IsEnemy( eTeam team ) const;

public:
	float	m_health = 10.0f;
	float	m_damage_multiplier = 1.0f;
	eTeam	m_team = ALLIED_TEAM;

private:
	static StatsComp s_prototype;

};
