#pragma once
#include "Game/Components/Component.hpp"

enum eQuestState
{
	STATE_INIT,
	STATE_ACCEPTED,
	STATE_FAILED,
	STATE_COMPLETE,
	STATE_FINISHED
};



struct QuestComp : public Component
{
public:
	QuestComp();
	~QuestComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

	std::string GetKillEnemiesText() const;
	std::string GetDialog() const;

public:
	// Determines if the quest can be given to a carrier
	bool is_active = false;

	std::string quest_name = "UNINITED QUEST NAME";
	std::string init_text = "UNINITED QUEST INIT";
	std::string accept_text = "UNINITED QUEST ACCEPT";
	std::string complete_text = "UNINITED QUEST COMPLETE";
	std::string fail_text = "UNINITED QUEST FAIL";

	// Completion requirements
	uint num_enemies_to_kill = 0;
	uint num_enemies_killed = 0;

	eQuestState state = STATE_INIT;

	EntityID open_exit_on_finished = -1;

private:
	static QuestComp s_prototype;

};
