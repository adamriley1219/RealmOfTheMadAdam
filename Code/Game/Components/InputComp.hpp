#pragma once
#include "Game/Components/Component.hpp"

#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"


struct InputComp : public Component
{
public:
	InputComp();
	~InputComp();

	KeyButtonState* GetKeyState( Key key ) const;

public:
	std::map<Key, KeyButtonState*> m_button_states;
	Vec2 m_move_dir = Vec2::ZERO;
};
