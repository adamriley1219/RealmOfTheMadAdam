#include "Game/Systems/RenderSystem.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Map.hpp"
#include "Game/EntityAdmin.hpp"

#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/BitmapFont.hpp"

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
	
	g_theRenderer->BindTextureView( 0, nullptr );

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
		SAFE_DELETE(tuple);
	}
	if( verts.size() > 0 )
	{
		g_theRenderer->DrawVertexArray( verts );
		verts.clear();
	}

	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelFixedFont" );
	g_theRenderer->BindTextureView( TEXTURE_SLOT_ALBEDO, font->GetTextureView() );

	for ( auto& ent_par : admin.m_entities )
	{
		Entity& entity = *(ent_par.second);
		QuestGiverComp* quest_giver = (QuestGiverComp*)entity.GetComponent( QUEST_GIVER_COMP );
		if( quest_giver )
		{
			//quest_giver->
		}
	}
}

//--------------------------------------------------------------------------
/**
* Update
*/
void RenderSystem::Update( float deltaTime ) const
{
	UNUSED( deltaTime );
	for( auto ent_pair : GetCurrentAdmin().m_entities )
	{
		Entity& entity = *(ent_pair.second);
		CameraComp* cam_comp = (CameraComp*)entity.GetComponent( CAMERA_COMP );
		TransformComp* trans_comp = (TransformComp*)entity.GetComponent( TRANSFORM_COMP );
		if( cam_comp && trans_comp && cam_comp->m_active )
		{
			cam_comp->m_camera.SetModelMatrix( Matrix44::MakeTranslation2D( trans_comp->m_transform.m_position ) );
			cam_comp->m_camera.SetColorTargetView(g_theRenderer->GetColorTargetView());
			cam_comp->m_camera.SetDepthTargetView(g_theRenderer->GetDepthTargetView());
			g_theRenderer->BeginCamera( &cam_comp->m_camera );
		}
	}
}
