#include "Game/Systems/UISystem.hpp"

#include "Game/EntityAdmin.hpp"
#include "Game/Entity.hpp"

#include "Game/Components/CameraComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/TriggerComp.hpp"

#include "Game/App.hpp"

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
	PROFILE_FUNCTION();
	EntityAdmin& admin = GetCurrentAdmin();

	Entity* carrier = admin.GetFirstWithComp( QUEST_CARRIER_COMP );

	if( carrier )
	{
		RenderCarrierUI( *carrier );

		for( Entity* giver : admin.GetAllWithComp( QUEST_GIVER_COMP ) )
		{
			RenderGiverUI( *giver );
			RenderGiverWithCarrierUI( *giver, *carrier );
		}

		for( Entity* trigger : admin.GetAllWithComp( TRIGGER_COMP ) )
		{
			RenderPortalWithCarrierUI( *trigger, *carrier );
		}
	}
}


//--------------------------------------------------------------------------
/**
* RenderCarrierUI
*/
void UISystem::RenderCarrierUI( Entity& carrier ) const
{
	EntityAdmin& admin = GetCurrentAdmin();
	QuestCarrierComp* quest_carrier_comp = (QuestCarrierComp*)carrier.GetComponent(QUEST_CARRIER_COMP);
	CameraComp* camera_comp = (CameraComp*)carrier.GetComponent(CAMERA_COMP);
	RenderComp* render_comp = (RenderComp*)carrier.GetComponent(RENDER_COMP);
	TransformComp* trans_comp = (TransformComp*)carrier.GetComponent(TRANSFORM_COMP);

	if( !( camera_comp && render_comp && trans_comp && quest_carrier_comp ) )
	{
		return;
	}

	// Hacking in FPS counter....
	// #TODO: Remove later
	//--------------------------------------------------------------------------
	AABB2 screen = g_gameConfigBlackboard.GetValue("screen", AABB2::ONE_BY_ONE);
	screen.AddPosition(trans_comp->m_transform.m_position);

	AABB2 carved_bot = screen.CarveBoxOffBottom(0.0f, 0.15f);
	uint fps = Clock::Master.GetFPS();
	float text_height = 0.1f;

	fps_array[cur_fps_idx] = fps;
	++cur_fps_idx;
	if( cur_fps_idx >= FPS_ARRAY_AVG_SIZE )
	{
		cur_fps_idx = 0;
	}

	uint avg_fps = 0;
	for( uint idx = 0; idx < FPS_ARRAY_AVG_SIZE; ++idx )
	{
		avg_fps += fps_array[idx];
	}
	avg_fps /= 30;

	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
	std::vector<Vertex_PCU>& verts = render_comp->m_verts_groups["SquirrelFixedFont"].verts;
	render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;

	font->AddVertsFor2DTextAlignedInBox(verts, text_height, Stringf("Average FPS: %u", avg_fps).c_str(), carved_bot, Vec2::ALIGN_BOTTOM_LEFT, BITMAP_MODE_UNCHANGED, m_alignment_modifier);
	carved_bot = screen.CarveBoxOffBottom(0.0f, 0.15f);
	font->AddVertsFor2DTextAlignedInBox(verts, text_height, Stringf("FPS: %u", fps).c_str(), carved_bot, Vec2::ALIGN_BOTTOM_LEFT, BITMAP_MODE_UNCHANGED, m_alignment_modifier);

	// Hack num entites
	// Num entities
	font->AddVertsFor2DTextAlignedInBox(verts, text_height, Stringf("Entities: %u", admin.GetNumEntites()).c_str(), screen.CarveBoxOffBottom(0.0f, 0.15f), Vec2::ALIGN_BOTTOM_LEFT, BITMAP_MODE_UNCHANGED, m_alignment_modifier);
	font->AddVertsFor2DTextAlignedInBox(verts, text_height, Stringf("Time: %.03f", g_theApp->GetGameClock()->GetTotalTime()).c_str(), screen.CarveBoxOffBottom(0.0f, 0.15f), Vec2::ALIGN_BOTTOM_LEFT, BITMAP_MODE_UNCHANGED, m_alignment_modifier);

	//--------------------------------------------------------------------------
	if( !camera_comp->m_active )
	{
		return;
	}

	// Check if we need to render for quest carriers
	screen.CarveBoxOffTop(0.0f, 0.2f);
	screen.CarveBoxOffRight(0.0f, 0.1f);

	AABB2 title_text_line = screen.CarveBoxOffTop(0.0f, 0.5f);
	const Vec2& QuestLogAlinement = Vec2::ALIGN_CENTER_RIGHT;
	eBitmapMode mode = BITMAP_MODE_UNCHANGED;
	font->AddVertsFor2DTextAlignedInBox( verts, text_height, "Quests", title_text_line, QuestLogAlinement, mode, m_alignment_modifier );
	for (QuestComp* quest_comp : quest_carrier_comp->quests)
	{
		if( quest_comp )
		{
			AABB2 text_line = screen.CarveBoxOffTop(0.0f, 0.1f);
			render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
			if( quest_comp->state >= STATE_COMPLETE )
			{
				font->AddVertsFor2DTextAlignedInBox( verts, text_height, Stringf("%s: Quest Complete", quest_comp->quest_name.c_str()).c_str(), text_line, QuestLogAlinement, mode, m_alignment_modifier );
			}
			else
			{
				font->AddVertsFor2DTextAlignedInBox( verts, text_height, Stringf("%s: %s", quest_comp->quest_name.c_str(), quest_comp->GetKillEnemiesText().c_str()).c_str(), text_line, QuestLogAlinement, mode, m_alignment_modifier );
			}
		}
	}
}

//--------------------------------------------------------------------------
/**
* RenderGiverUI
*/
void UISystem::RenderGiverUI( Entity& giver ) const
{
	RenderComp* giver_render_comp = (RenderComp*)giver.GetComponent( RENDER_COMP );
	TransformComp* giver_trans_comp = (TransformComp*)giver.GetComponent( TRANSFORM_COMP );
	QuestGiverComp* quest_giver_comp = (QuestGiverComp*)giver.GetComponent( QUEST_GIVER_COMP );
	PhysicsComp* giver_physics_comp = (PhysicsComp*)giver.GetComponent( PHYSICS_COMP );

	if (!(quest_giver_comp && giver_render_comp && giver_trans_comp && giver_physics_comp))
	{
		return;
	}

	float radius = giver_physics_comp->m_radius;
	QuestComp* giver_quest = quest_giver_comp->GetCurrentQuest();

	// Render quest icon above giver head
	if( giver_quest )
	{
		switch( giver_quest->state )
		{
		case STATE_FAILED:
			break;
		case STATE_COMPLETE:
			AddVertsForAABB2D(giver_render_comp->m_verts_groups["Data/Images/Extras_4x4.png"].verts, AABB2( radius, radius, giver_trans_comp->m_transform.m_position + Vec2::UP * .35f ), Rgba::RED, Vec2( 0.75f, 0.25f ), Vec2( 1.0f, 0.5f ) );
			break;
		case STATE_FINISHED:
			break;
		case STATE_INIT:
		case STATE_ACCEPTED:
		default:
			AddVertsForAABB2D(giver_render_comp->m_verts_groups["Data/Images/Extras_4x4.png"].verts, AABB2( radius, radius, giver_trans_comp->m_transform.m_position + Vec2::UP * .35f ), Rgba::CYAN, Vec2( 0.75f, 0.0f ), Vec2( 1.0f, 0.25f ) );
			break;
		}
	}
}

//--------------------------------------------------------------------------
/**
* RenderGiverWithCarrierUI
*/
void UISystem::RenderGiverWithCarrierUI( Entity& giver, Entity& carrier ) const
{
	RenderComp* giver_render_comp = (RenderComp*)giver.GetComponent(RENDER_COMP);
	TransformComp* giver_trans_comp = (TransformComp*)giver.GetComponent(TRANSFORM_COMP);
	QuestGiverComp* quest_giver_comp = (QuestGiverComp*)giver.GetComponent(QUEST_GIVER_COMP);
	PhysicsComp* giver_physics_comp = (PhysicsComp*)giver.GetComponent(PHYSICS_COMP);

	if( !( quest_giver_comp && giver_render_comp && giver_trans_comp && giver_physics_comp ) )
	{
		return;
	}

	QuestComp* giver_quest = quest_giver_comp->GetCurrentQuest();
	float radius = giver_physics_comp->m_radius;

	InteractComp* carrier_interact_comp = (InteractComp*)carrier.GetComponent(INTERACT_COMP);
	TransformComp* carrier_trans_comp = (TransformComp*)carrier.GetComponent(TRANSFORM_COMP);

	if( !(carrier_trans_comp && carrier_interact_comp ) )
	{
		return;
	}

	Vec2 displcement = carrier_trans_comp->m_transform.m_position - giver_trans_comp->m_transform.m_position;
	bool in_range = displcement.GetLengthSquared() < carrier_interact_comp->m_interact_range * carrier_interact_comp->m_interact_range;
	if (in_range)
	{
		BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
		std::vector<Vertex_PCU>& verts = giver_render_comp->m_verts_groups["SquirrelFixedFont"].verts;

		// Render Dialog Box
		if (quest_giver_comp->being_interacted_with)
		{
			// Draw text box
			AABB2 screen = g_gameConfigBlackboard.GetValue("screen", AABB2::ONE_BY_ONE);

			screen.CarveBoxOffBottom(0.65f);
			screen.CarveBoxOffLeft(0.1f);
			screen.CarveBoxOffRight(0.5f);
			screen.CarveBoxOffTop(0.0f, 0.3f);

			screen.AddPosition(carrier_trans_comp->m_transform.m_position);

			AABB2 top_line = screen.CarveBoxOffTop(0.0f, 0.05f);
			AABB2 bottom_line = screen.CarveBoxOffBottom(0.0f, 0.05f);
			AABB2 left_line = screen.CarveBoxOffLeft(0.0f, 0.05f);
			AABB2 right_line = screen.CarveBoxOffRight(0.0f, 0.05f);

			AddVertsForAABB2D(giver_render_comp->m_verts_groups[""].verts, screen, Rgba(0.2f, 0.2f, 0.2f, 0.8f));

			Rgba boarder_color = Rgba(0.05f, 0.05f, 0.05f, 0.9f);
			AddVertsForAABB2D(giver_render_comp->m_verts_groups[""].verts, top_line, boarder_color);
			AddVertsForAABB2D(giver_render_comp->m_verts_groups[""].verts, bottom_line, boarder_color);
			AddVertsForAABB2D(giver_render_comp->m_verts_groups[""].verts, left_line, boarder_color);
			AddVertsForAABB2D(giver_render_comp->m_verts_groups[""].verts, right_line, boarder_color);

			screen.CarveBoxOffTop(0.0f, 0.05f);
			screen.CarveBoxOffBottom(0.0f, 0.05f);
			screen.CarveBoxOffLeft(0.0f, 0.05f);
			screen.CarveBoxOffRight(0.0f, 0.05f);
			
			float text_height = .1f;

			if (giver_quest)
			{
				font->AddVertsFor2DTextAlignedInBox( verts, text_height, giver_quest->GetDialog().c_str(), screen, Vec2::ALIGN_TOP_LEFT, BITMAP_MODE_SHRINK_TO_FIT, m_alignment_modifier );
					
			}
			else
			{
				font->AddVertsFor2DTextAlignedInBox( verts, text_height, "Nothing to say. (no quest to give)", screen, Vec2::ALIGN_TOP_LEFT, BITMAP_MODE_SHRINK_TO_FIT, m_alignment_modifier );
			}
		}

		giver_render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
		font->AddVertsFor2DText( verts, Vec2( giver_trans_comp->m_transform.m_position ) - Vec2( radius * .5f, radius * 4.0f ) * .5f, .25f, "F", 0.5f, Rgba( 1.0f, .9f, .6f ) );
	}
}

//--------------------------------------------------------------------------
/**
* RenderExitWithCarrierUI
*/
void UISystem::RenderPortalWithCarrierUI( Entity& portal, Entity& carrier ) const
{
	TriggerComp* portal_trigger_comp	= (TriggerComp*) portal.GetComponent( TRIGGER_COMP );
	RenderComp* portal_render_comp		= (RenderComp*) portal.GetComponent( RENDER_COMP );
	TransformComp* portal_trans_comp	= (TransformComp*) portal.GetComponent( TRANSFORM_COMP );
	PhysicsComp* portal_phyx_comp		= (PhysicsComp*) portal.GetComponent( PHYSICS_COMP );

	TransformComp* carrier_trans_comp	= (TransformComp*) carrier.GetComponent( TRANSFORM_COMP );
	InteractComp* carrier_interact_comp = (InteractComp*) carrier.GetComponent( INTERACT_COMP );

	if( !( portal_trigger_comp && portal_render_comp && portal_trans_comp
		&& carrier_interact_comp && carrier_trans_comp ) )
	{
		return;
	}

	// Check if acting portal/exit 
	if( !portal_trigger_comp->m_portal_active && !portal_trigger_comp->m_transfer_map )
	{
		return;
	}

	Vec2 displcement = carrier_trans_comp->m_transform.m_position - portal_trans_comp->m_transform.m_position;
	bool in_range = displcement.GetLengthSquared() < portal_phyx_comp->m_radius;
	float radius = portal_phyx_comp->m_radius;

	if( in_range )
	{
		BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile("SquirrelFixedFont");
		portal_render_comp->m_verts_groups["SquirrelFixedFont"].is_text = true;
		Vec2 text_position = Vec2(portal_trans_comp->m_transform.m_position) - Vec2(radius * .5f, radius * 4.0f) * .5f;
		font->AddVertsFor2DText(portal_render_comp->m_verts_groups["SquirrelFixedFont"].verts, text_position, .25f, "Press F to Exit", 0.5f, Rgba(1.0f, .9f, .6f));
	}
}
