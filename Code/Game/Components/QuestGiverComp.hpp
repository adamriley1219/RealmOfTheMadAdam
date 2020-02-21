#pragma once
#include "Game/Components/Component.hpp"



struct QuestGiverComp : public Component
{
public:
	QuestGiverComp();
	~QuestGiverComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();


	std::string GetKillEnemiesText() const;
	std::string GetDialog() const;

public:
	uint map_loc_id;
	std::string quest_name = "UNINITED QUEST NAME";
	std::string init_text = "UNINITED QUEST INIT";
	std::string accept_text = "UNINITED QUEST ACCEPT";
	std::string complete_text = "UNINITED QUEST COMPLETE";
	std::string fail_text = "UNINITED QUEST FAIL";

	uint num_enemies_to_kill = 0;
	uint num_enemies_killed = 0;

	bool triggered = false;
	bool player_in_range = false;
	bool complete = false;
};
