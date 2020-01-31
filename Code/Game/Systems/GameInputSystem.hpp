#pragma once
#include "Game/Systems/System.hpp"



class GameInpuSystem : public System
{
public:
	GameInpuSystem();
	~GameInpuSystem();

	void Update( float deltaTime ) const;

private:

};
