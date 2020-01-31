#pragma once
#include "Game/GameCommon.hpp"

enum eComponentType
{
	UNIDENTIFIED_COMP = -1,
	INPUT_COMP,
	NAME_COMP,
	PHYSICS_COMP,
	RENDER_COMP,
	TRANSFORM_COMP,
	UI_COMP
};


struct Component
{
public:
	Component();
	virtual ~Component();

	void GetClaimed( EntityID owner );

public:
	eComponentType m_type = UNIDENTIFIED_COMP;

public:
	EntityID m_owner = 0;
};
