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
	UI_COMP,
	QUEST_GIVER_COMP,
	QUEST_CARRIER_COMP,
	INTENT_COMP,
	AI_COMP,
	CAMERA_COMP,
	INTERACT_COMP,
	NUM_COMP_TYPES
};


struct Component
{
public:
	Component();
	virtual ~Component();

	virtual void GetClaimed( EntityID owner );

public:
	EntityID m_owner = 0;
//	ComponentID	m_id = 0;

public:
	virtual void Copy( const Component* copyFrom ) = 0;
	virtual eComponentType GetType() = 0;

};
