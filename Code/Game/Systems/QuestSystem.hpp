#pragma once
#include "Game/Systems/System.hpp"


class QuestSystem : public System
{
public:
	QuestSystem();
	~QuestSystem();

	void Update( float deltaTime ) const;

private:

};
