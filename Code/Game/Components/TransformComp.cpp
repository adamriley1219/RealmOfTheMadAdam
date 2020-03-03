#include "Game/Components/TransformComp.hpp"

TransformComp TransformComp::s_prototype;

//--------------------------------------------------------------------------
/**
* TransformComp
*/
TransformComp::TransformComp()
	: Component()
{
	m_transform.m_scale = Vec2( 1.0f, 1.0f );
	m_transform.m_position = Vec2( 0.0f, 0.0f );
	m_transform.m_rotation = 0.0f;
}

//--------------------------------------------------------------------------
/**
* TransformComp
*/
TransformComp::TransformComp( float x, float y )
	: Component(), m_transform( Vec2( x, y ) )
{
	
}

//--------------------------------------------------------------------------
/**
* ~TransformComp
*/
TransformComp::~TransformComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void TransformComp::Copy( const Component* copyFrom )
{
	*this = *( (TransformComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType TransformComp::GetType()
{
	return TRANSFORM_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void TransformComp::Reset()
{
	*this = s_prototype;
}
