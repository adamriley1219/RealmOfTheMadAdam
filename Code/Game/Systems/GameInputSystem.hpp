#pragma once
#include "Game/Systems/System.hpp"
#include "Game/Components/InputComp.hpp"

struct IntentComp;


//--------------------------------------------------------------------------
// Player hotkeys
//--------------------------------------------------------------------------
extern Key INTERACT_KEY;
extern Key FIRE_KEY;
extern Key FIRE_SECONDARY_KEY;
extern Key MOVE_LEFT;
extern Key MOVE_RIGHT;
extern Key MOVE_UP;
extern Key MOVE_DOWN;
//--------------------------------------------------------------------------

struct InputTuple
{
	InputComp* input_comp;
	IntentComp* intent_comp;
};

class GameInpuSystem : public System
{
public:
	GameInpuSystem();
	~GameInpuSystem();

	void Update( float deltaTime ) const;

private:

};
