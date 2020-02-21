#pragma once
#include "Game/Systems/System.hpp"

struct QuestGiverComp;

class QuestSystem : public System
{
public:
	QuestSystem();
	~QuestSystem();

	void Update( float deltaTime ) const;

	void TriggerQuestComplete( QuestGiverComp* giver ) const;
	void TriggerQuestAccepted( QuestGiverComp* giver ) const;

private:

};
