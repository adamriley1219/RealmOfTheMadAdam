#include "Game/Systems/UISystem.hpp"

#include "Game/EntityAdmin.hpp"

#include "Game/Components/CameraComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/InteractComp.hpp"

#include "Engine/Core/Strings/NamedStrings.hpp"
#include "Engine/Core/Time/Clock.hpp"

#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/RenderContext.hpp"

//--------------------------------------------------------------------------
/**
* UISystem
*/
UISystem::UISystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~UISystem
*/
UISystem::~UISystem()
{

}

//--------------------------------------------------------------------------
/**
* Render
*/
void UISystem::Render() const
{
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{
		if( !entity.m_claimed )
		{
			continue;
		}
		CameraComp* camera_comp = (CameraComp*)entity.GetComponent( CAMERA_COMP );
		RenderComp* render_comp = (RenderComp*)entity.GetComponent( RENDER_COMP );
		TransformComp* trans_comp = (TransformComp*)entity.GetComponent( TRANSFORM_COMP );
		
		if( camera_comp && camera_comp->m_active )
		{

			if( render_comp && trans_comp )
			{
				// Hacking in FPS counter....
				//--------------------------------------------------------------------------
				AABB2 screen = g_gameConfigBlackboard.GetValue("screen", AABB2::ONE_BY_ONE);
				screen.AddPosition( trans_comp->m_transform.m_position );
				uint fps = Clock::Master.GetFPS();

				BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
				render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
				font->AddVertsFor2DTextAlignedInBox(render_comp->m_verts_groups["SquirrelFixedFont"].verts, 0.1f, ToString(fps).c_str(), screen, Vec2::ALIGN_BOTTOM_LEFT, BITMAP_MODE_UNCHANGED, 0.7f);
				//--------------------------------------------------------------------------


				// Check if we need to render for quest carriers
				QuestCarrierComp* quest_carrier_comp = (QuestCarrierComp*)entity.GetComponent(QUEST_CARRIER_COMP);
				if( quest_carrier_comp )
				{
					AABB2 screen = g_gameConfigBlackboard.GetValue("screen", AABB2::ONE_BY_ONE);
					screen.AddPosition(trans_comp->m_transform.m_position);
					screen.CarveBoxOffTop(0.0f, 0.3f);
					screen.CarveBoxOffRight(0.0f, 0.1f);

					for (QuestGiverComp* quest_giver : quest_carrier_comp->quest_givers)
					{
						AABB2 text_line = screen.CarveBoxOffTop(0.0f, 0.1f);

						BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
						render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
						if (quest_giver->complete)
						{
							font->AddVertsFor2DTextAlignedInBox(render_comp->m_verts_groups["SquirrelFixedFont"].verts, 0.1f, Stringf("%s: Quest Complete", quest_giver->quest_name.c_str()).c_str(), text_line, Vec2::ALIGN_CENTER_RIGHT, BITMAP_MODE_SHRINK_TO_FIT, .7f);
						}
						else
						{
							font->AddVertsFor2DTextAlignedInBox(render_comp->m_verts_groups["SquirrelFixedFont"].verts, 0.1f, Stringf("%s: %s", quest_giver->quest_name.c_str(), quest_giver->GetKillEnemiesText().c_str()).c_str(), text_line, Vec2::ALIGN_CENTER_RIGHT, BITMAP_MODE_SHRINK_TO_FIT, .7f);
						}
					}
				}
			}
		}

		// Check to see if we need to render for quest givers
		QuestGiverComp* quest_giver_comp = (QuestGiverComp*)entity.GetComponent(QUEST_GIVER_COMP);
		if (quest_giver_comp)
		{
			for (auto other_entity : GetCurrentAdmin().m_entities)
			{
				if (!other_entity.m_claimed || other_entity.m_id == entity.m_id)
				{
					// Don't check on self
					continue;
				}
				InteractComp* interact_comp = (InteractComp*)other_entity.GetComponent(INTERACT_COMP);
				TransformComp* other_trans_comp = (TransformComp*)other_entity.GetComponent(TRANSFORM_COMP);
				if( other_trans_comp && interact_comp )
				{
					Vec2 displcement = other_trans_comp->m_transform.m_position - trans_comp->m_transform.m_position;
					bool in_range = displcement.GetLengthSquared() < interact_comp->m_interact_range * interact_comp->m_interact_range;
					if (in_range)
					{
						// Render Dialog Box
						if (quest_giver_comp->triggered)
						{
							// Draw text box
							AABB2 screen = g_gameConfigBlackboard.GetValue("screen", AABB2::ONE_BY_ONE);

							screen.CarveBoxOffBottom(0.65f);
							screen.CarveBoxOffLeft(0.1f);
							screen.CarveBoxOffRight(0.5f);
							screen.CarveBoxOffTop(0.0f, 0.3f);

							screen.AddPosition(other_trans_comp->m_transform.m_position);

							AABB2 top_line = screen.CarveBoxOffTop(0.0f, 0.05f);
							AABB2 bottom_line = screen.CarveBoxOffBottom(0.0f, 0.05f);
							AABB2 left_line = screen.CarveBoxOffLeft(0.0f, 0.05f);
							AABB2 right_line = screen.CarveBoxOffRight(0.0f, 0.05f);

							AddVertsForAABB2D(render_comp->m_verts_groups[""].verts, screen, Rgba(0.2f, 0.2f, 0.2f, 0.8f));

							Rgba boarder_color = Rgba(0.05f, 0.05f, 0.05f, 0.9f);
							AddVertsForAABB2D(render_comp->m_verts_groups[""].verts, top_line, boarder_color);
							AddVertsForAABB2D(render_comp->m_verts_groups[""].verts, bottom_line, boarder_color);
							AddVertsForAABB2D(render_comp->m_verts_groups[""].verts, left_line, boarder_color);
							AddVertsForAABB2D(render_comp->m_verts_groups[""].verts, right_line, boarder_color);

							screen.CarveBoxOffTop(0.0f, 0.05f);
							screen.CarveBoxOffBottom(0.0f, 0.05f);
							screen.CarveBoxOffLeft(0.0f, 0.05f);
							screen.CarveBoxOffRight(0.0f, 0.05f);

							BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
							render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
							font->AddVertsFor2DTextAlignedInBox(render_comp->m_verts_groups["SquirrelFixedFont"].verts, 0.1f, quest_giver_comp->GetDialog().c_str(), screen, Vec2::ALIGN_TOP_LEFT, BITMAP_MODE_SHRINK_TO_FIT, 0.7f);
						}
					}
				}
			}
		}
	}
}
