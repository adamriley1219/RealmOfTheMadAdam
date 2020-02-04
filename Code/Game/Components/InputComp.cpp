#include "Game/Components/InputComp.hpp"
#include "Game/Systems/GameInputSystem.hpp"

//--------------------------------------------------------------------------
/**
* NameComp
*/
InputComp::InputComp()
	: Component()
{
	m_type = INPUT_COMP;

	m_button_states[INTERACT_KEY] = new KeyButtonState();
	m_button_states[FIRE_KEY] = new KeyButtonState();
	m_button_states[FIRE_SECONDARY_KEY] = new KeyButtonState();
	m_button_states[MOVE_LEFT] = new KeyButtonState();
	m_button_states[MOVE_RIGHT] = new KeyButtonState();
	m_button_states[MOVE_UP] = new KeyButtonState();
	m_button_states[MOVE_DOWN] = new KeyButtonState();
}

//--------------------------------------------------------------------------
/**
* ~NameComp
*/
InputComp::~InputComp()
{

}

//--------------------------------------------------------------------------
/**
* GetKeyState
*/
KeyButtonState* InputComp::GetKeyState( Key key ) const
{
	if( m_button_states.find(key) != m_button_states.end() )
	{
		return m_button_states.at( key );
	}
	return nullptr;
}
