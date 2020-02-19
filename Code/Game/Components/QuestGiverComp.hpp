#pragma once
#include "Game/Components/Component.hpp"



struct QuestGiverComp : public Component
{
public:
	QuestGiverComp();
	~QuestGiverComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();

public:
	uint map_loc_id;
	std::string quest_name = "UNINITED QUEST NAME";
	std::string init_text = "UNINITED QUEST INIT";
	std::string accept_text = "UNINITED QUEST ACCEPT";
	std::string complete_text = "UNINITED QUEST COMPLETE";
	std::string fail_text = "UNINITED QUEST FAIL";

	bool player_in_range = false;
	bool complete = false;
};
