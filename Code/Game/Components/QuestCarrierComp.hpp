#pragma once
#include "Game/Components/Component.hpp"

struct QuestGiverComp;

struct QuestCarrierComp : public Component
{
public:
	QuestCarrierComp();
	~QuestCarrierComp();

	void AddQuest( QuestGiverComp* quest_giver );

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();

public:
	std::vector< QuestGiverComp* > quest_givers;

};
