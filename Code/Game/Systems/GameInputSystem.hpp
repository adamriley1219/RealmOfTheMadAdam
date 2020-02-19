#pragma once
#include "Game/Systems/System.hpp"
#include "Game/Components/InputComp.hpp"

struct IntentComp;

class GameInputSystem : public System
{
public:
	GameInputSystem();
	~GameInputSystem();

	void Update( float deltaTime ) const;

private:
	Key GetKeyFromHotkey( eHotkeys hotkey ) const;
};
