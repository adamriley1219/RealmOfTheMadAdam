#include "Game/Components/NameComp.hpp"

//--------------------------------------------------------------------------
/**
* NameComp
*/
NameComp::NameComp()
	: Component()
{
	m_type = NAME_COMP;
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
