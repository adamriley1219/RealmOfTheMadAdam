#pragma once
#include "Game/Components/Component.hpp"

struct QuestComp;

struct QuestGiverComp : public Component
{
public:
	QuestGiverComp();
	~QuestGiverComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();


public:
	// Returns the first quest that tis marked active 
	QuestComp* GetCurrentQuest() const;
	
	// Puts the quest at the end of the list of quests given
	void AddQuest( QuestComp* quest_to_add );

	void RemoveQuest( QuestComp* quest_to_remove );

public:
	uint map_loc_id;
	bool being_interacted_with = false;
	bool player_in_range = false;

	std::vector<QuestComp*> quests;

private:
	static QuestGiverComp s_prototype;

};
