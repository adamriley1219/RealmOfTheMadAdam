#pragma once
#include "Game/Systems/System.hpp"

struct QuestGiverComp;
struct QuestComp;
class Entity;

class QuestSystem : public System
{
public:
	QuestSystem();
	~QuestSystem();

	void Update( float deltaTime ) const;

private:
	void UpdateQuestGiverWithCarrier( Entity& giver, Entity& carrier ) const;
	void UpdateQuestCarrier( Entity& carrier ) const;
	void UpdateQuest( QuestComp& quest ) const;
	void UpdateQuestStateWithTriggers( QuestComp& quest ) const;

private:
	void QuestComplete( QuestComp& giver ) const;
	void QuestAccepted( QuestComp& giver ) const;
	void QuestFinished( QuestComp& giver ) const;

};
