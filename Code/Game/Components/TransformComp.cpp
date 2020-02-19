#include "Game/Components/TransformComp.hpp"

//--------------------------------------------------------------------------
/**
* TransformComp
*/
TransformComp::TransformComp()
	: Component()
{
	
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
