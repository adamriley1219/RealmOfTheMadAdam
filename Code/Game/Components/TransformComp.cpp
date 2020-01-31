#include "Game/Components/TransformComp.hpp"

//--------------------------------------------------------------------------
/**
* TransformComp
*/
TransformComp::TransformComp()
	: Component()
{
	m_type = TRANSFORM_COMP;
}

//--------------------------------------------------------------------------
/**
* TransformComp
*/
TransformComp::TransformComp( float x, float y )
	: Component(), m_transform( Vec2( x, y ) )
{
	m_type = TRANSFORM_COMP;
}

//--------------------------------------------------------------------------
/**
* ~TransformComp
*/
TransformComp::~TransformComp()
{

}
