#pragma once
#include "Game/Components/Component.hpp"

struct QuestGiverComp;

struct QuestCarrierComp : public Component
{
public:
	QuestCarrierComp();
	~QuestCarrierComp();

	void AddQuest( QuestGiverComp* quest_giver );

public:
	std::vector< QuestGiverComp* > quest_givers;

};