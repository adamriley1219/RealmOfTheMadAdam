#include "Game/Systems/RenderSystem.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Map.hpp"
#include "Game/EntityAdmin.hpp"

#include "Engine/Renderer/RenderContext.hpp"


//--------------------------------------------------------------------------
/**
* RenderSystem
*/
RenderSystem::RenderSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~RenderSystem
*/
RenderSystem::~RenderSystem()
{

}

//--------------------------------------------------------------------------
/**
* Render
*/
void RenderSystem::Render() const
{
	Map* map = GetCurrentMap();
	EntityAdmin& admin = map->m_admin;
	
	std::vector<Vertex_PCU> verts;
	for( auto ent_par : admin.m_entities )
	{
		RenderSystemTuple* tuple = admin.GetRenderTuple( ent_par.first );
		if( tuple )
		{
			// Can render something
			now you can render when you come back
		}
	}
	if( verts.size() > 0 )
	{
		g_theRenderer->DrawVertexArray( verts );
	}
}
