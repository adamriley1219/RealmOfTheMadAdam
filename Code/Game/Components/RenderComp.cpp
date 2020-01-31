#include "Game/Components/RenderComp.hpp"

//--------------------------------------------------------------------------
/**
* RenderComp
*/
RenderComp::RenderComp()
	: Component()
{
	AddVertsForRing2D( m_verts, Vec2::ZERO, .25f, .01f, Rgba::WHITE, 8 );
	m_type = RENDER_COMP;
}

//--------------------------------------------------------------------------
/**
* ~RenderComp
*/
RenderComp::~RenderComp()
{

}
