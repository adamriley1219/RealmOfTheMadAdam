#include "Game/Components/InputComp.hpp"
#include "Game/Systems/GameInputSystem.hpp"

InputComp InputComp::s_prototype;

//--------------------------------------------------------------------------
/**
* NameComp
*/
InputComp::InputComp()
	: Component()
{
	
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
KeyButtonState* InputComp::GetKeyState( eHotkeys key )
{
	return &m_button_states[key];
}

//--------------------------------------------------------------------------
/**
* Copy
*/
void InputComp::Copy(const Component* copyFrom)
{
	*this = *( (InputComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType InputComp::GetType()
{
	return INPUT_COMP;
}

