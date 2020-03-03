#include "Game/Components/RenderComp.hpp"

RenderComp RenderComp::s_prototype;

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

//--------------------------------------------------------------------------
/**
* Reset
*/
void RenderComp::Reset()
{
	*this = s_prototype;
}

