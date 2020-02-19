#include "Game/Components/RenderComp.hpp"

//--------------------------------------------------------------------------
/**
* RenderComp
*/
RenderComp::RenderComp()
	: Component()
{

}

//--------------------------------------------------------------------------
/**
* ~RenderComp
*/
RenderComp::~RenderComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void RenderComp::Copy(const Component* copyFrom)
{
	*this = *( (RenderComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType RenderComp::GetType()
{
	return RENDER_COMP;
}
