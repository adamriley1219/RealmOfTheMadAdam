#include "Game/Components/NameComp.hpp"

//--------------------------------------------------------------------------
/**
* NameComp
*/
NameComp::NameComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* NameComp
*/
NameComp::NameComp( const std::string& given_name )
	: m_name( given_name )
{
	
}

//--------------------------------------------------------------------------
/**
* ~NameComp
*/
NameComp::~NameComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void NameComp::Copy( const Component* copyFrom )
{
	*this = *( (NameComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType NameComp::GetType()
{
	return NAME_COMP;
}
