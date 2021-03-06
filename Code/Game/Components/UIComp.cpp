#include "Game/Components/UIComp.hpp"


UIComp UIComp::s_prototype;

//--------------------------------------------------------------------------
/**
* UIComp
*/
UIComp::UIComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
UIComp::~UIComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void UIComp::Copy( const Component* copyFrom )
{
	*this = *( (UIComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType UIComp::GetType()
{
	return UI_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void UIComp::Reset()
{
	*this = s_prototype;
}
