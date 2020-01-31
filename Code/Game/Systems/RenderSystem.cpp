#include "Game/Systems/RenderSystem.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Map.hpp"
#include "Game/EntityAdmin.hpp"

#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"

#include "Engine/Renderer/RenderContext.hpp"

#include "Engine/Math/Matrix44.hpp"

#include "Engine/Physics/Transform2D.hpp"


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
			Matrix44 trans_matrix = tuple->transform_comp->m_transform.GetTransformMatrix();
			for( Vertex_PCU vert : tuple->render_comp->m_verts )
			{
				vert.position = trans_matrix.TransformPosition3D( vert.position );
				verts.push_back( vert );
			}
		}
	}
	if( verts.size() > 0 )
	{
		g_theRenderer->DrawVertexArray( verts );
	}
}
