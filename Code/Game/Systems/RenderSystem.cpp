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
	
	std::map<std::string, Vertex_Info_Group> texture_names_to_group_infos;

	
	// Go get everything the entities want to render
	std::vector<Vertex_PCU> verts;
	for( auto entity : admin.m_entities )
	{
		if (!entity.m_claimed)
		{
			continue;
		}
		
		TransformComp* trans_comp = (TransformComp*)admin.GetComponent(entity.m_id, TRANSFORM_COMP);
		RenderComp* render_comp = (RenderComp*)admin.GetComponent( entity.m_id, RENDER_COMP );
		if( trans_comp && render_comp )
		{
			// Can render something
			Matrix44 main_transform = render_comp->m_main_group.transform.GetTransformMatrix();
			main_transform = main_transform.Concatenate( trans_comp->m_transform.GetTransformMatrix() );
			texture_names_to_group_infos[render_comp->m_main_texture].is_text = false; // Cant have text as the main path
			for ( Vertex_PCU vert : render_comp->m_main_group.verts )
			{
				vert.position = main_transform.TransformPosition3D( vert.position );
				texture_names_to_group_infos[render_comp->m_main_texture].verts.push_back( vert );
			}

			for( auto& group_pair : render_comp->m_verts_groups )
			{
				Vertex_Info_Group& group = (group_pair.second);
				const std::string& file_path = group_pair.first; 
				std::vector<Vertex_PCU>& group_verts = group.verts;

				if( group_verts.size() > 0 )
				{
					Matrix44 trans_matrix = group.transform.GetTransformMatrix();
					texture_names_to_group_infos[file_path].is_text = group.is_text;
					// Intended not to use a reference but a copy.
					// We are changing the position according to the matrix.
					for( Vertex_PCU vert : group_verts )
					{
						vert.position = trans_matrix.TransformPosition3D( vert.position );
						texture_names_to_group_infos[file_path].verts.push_back( vert );
					}

					group_verts.clear();
				}
			}
		}
	}

	// Time to render everything
	for( auto& to_render_pair : texture_names_to_group_infos )
	{
		const std::string& file_name = to_render_pair.first;
		Vertex_Info_Group& group = to_render_pair.second;
		if( group.verts.size() > 0 )
		{
			if( file_name == "" )
			{
				g_theRenderer->BindTextureViewWithSampler( TEXTURE_SLOT_ALBEDO, nullptr );
			}
			else
			{
				if( group.is_text )
				{
					BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile( file_name.c_str() );
					g_theRenderer->BindTextureViewWithSampler( TEXTURE_SLOT_ALBEDO, font->GetTextureView() );
				}
				else
				{
					g_theRenderer->BindTextureView( TEXTURE_SLOT_ALBEDO, file_name.c_str() );
				}
			}
			g_theRenderer->SetBlendMode( eBlendMode::BLEND_MODE_ALPHA );
			g_theRenderer->DrawVertexArray( group.verts );
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
	for( auto entity : GetCurrentAdmin().m_entities )
	{
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
