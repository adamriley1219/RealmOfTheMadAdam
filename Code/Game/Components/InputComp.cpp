#include "Game/Components/InputComp.hpp"

//--------------------------------------------------------------------------
/**
* NameComp
*/
InputComp::InputComp()
	: Component()
{
	m_type = INPUT_COMP;

	m_button_states[KEY_A] = new KeyButtonState();
	m_button_states[KEY_W] = new KeyButtonState();
	m_button_states[KEY_S] = new KeyButtonState();
	m_button_states[KEY_D] = new KeyButtonState();
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
