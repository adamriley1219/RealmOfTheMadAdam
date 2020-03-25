#pragma once
#include "Game/Components/Component.hpp"

struct QuestComp;

struct QuestCarrierComp : public Component
{
public:
	QuestCarrierComp();
	~QuestCarrierComp();

	void AddQuest( QuestComp* quest_giver );
	void RemoveQuest( QuestComp* quest_giver );

	virtual void Copy( const Component* copyFrom );
	virtual eComponentType GetType();
	virtual void Reset();

	bool HasQuest( QuestComp* quest_giver );

public:
	std::vector< QuestComp* > quests;

private:
	static QuestCarrierComp s_prototype;

};
