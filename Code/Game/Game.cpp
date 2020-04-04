#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/Debug/DebugRenderSystem.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"

#include "Engine/Core/Debug/DevConsole.hpp"
#include "Engine/Core/Strings/NamedStrings.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Time/Clock.hpp"
#include "Engine/Memory/Debug/Profiler.hpp"

#include "Engine/ImGUI/ImGUISystem.hpp"
#include "Engine/ImGUI/ImGUI_Interfaces/ProfilerDisplay.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"
#include "Game/Map.hpp"
#include "Game/EntityAdmin.hpp"

#include "Game/Systems/CombatSystem.hpp"
#include "Game/Systems/GameInputSystem.hpp"
#include "Game/Systems/GamePhysicsSystem.hpp"
#include "Game/Systems/QuestSystem.hpp"
#include "Game/Systems/TriggerSystem.hpp"
#include "Game/Systems/UISystem.hpp"
#include "Game/Systems/RenderSystem.hpp"
#include "Game/Systems/MovementSystem.hpp"
#include "Game/Systems/AISystem.hpp"

#include "Game/Components/InputComp.hpp"
#include "Game/Components/NameComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/UIComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/QuestComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/AIComp.hpp"
#include "Game/Components/StatsComp.hpp"
#include "Game/Components/TriggerComp.hpp"


#include <vector>

#include <Math.h>
//--------------------------------------------------------------------------
/**
* Game
*/
Game::Game()
{

}

//--------------------------------------------------------------------------
/**
* ~Game
*/
Game::~Game()
{

}

//--------------------------------------------------------------------------
/**
* GameStartup
*/
void Game::Startup()
{
	m_shader = g_theRenderer->CreateOrGetShaderFromXML( "Data/Shaders/shader.xml" );
	g_theRenderer->m_shader = m_shader;

	m_DevColsoleCamera.SetOrthographicProjection( Vec2( -100.0f, -50.0f ), Vec2( 100.0f,  50.0f ) );
	m_DevColsoleCamera.SetModelMatrix( Matrix44::IDENTITY );

	TileDefinition::InitiolizeDefinitions();

	m_CurentCamera.SetOrthographicProjection(Vec2(), Vec2( 10, 5 ));

	m_terrain_sheet = new SpriteSheet( (TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile("Data/Images/Terrain_8x8.png", true), IntVec2( 8, 8 ), Vec2::ALIGN_CENTERED, Vec2::ZERO );

	EntityAdmin::GetMaster();

	EntityAdmin::GetMaster()->m_systems.push_back( new GameInputSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new AISystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new GamePhysicsSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new CombatSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new QuestSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new TriggerSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new MovementSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new UISystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new RenderSystem() );

	m_map_id = 0;
	m_maps[0] = new Map( 0, IntVec2( 10, 10 ), m_terrain_sheet, TileType::GRASS_TILE, TileType::STONE_TILE );
	m_maps[1] = new Map( 1, IntVec2( 50, 50 ), m_terrain_sheet, TileType::SAND_TILE, TileType::STONE_TILE );
	m_maps[2] = new Map( 2, IntVec2( 5, 7 ), m_terrain_sheet, TileType::MUD_TILE, TileType::STONE_TILE );

	Vec2 bot_left_uvs;
	Vec2 top_right_uvs;

	SpriteSheet exit_sheet((TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile("Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png"), IntVec2(18, 4));
	const SpriteDefinition& exit_sdef = exit_sheet.GetSpriteDefinition(4, 1);
	exit_sdef.GetUVs(bot_left_uvs, top_right_uvs);


	//--------------------------------------------------------------------------
	// Exit map 2-1

	Entity* exit_2_1 = m_maps[2]->m_admin.CreateEntity();
	((NameComp*)exit_2_1->AddComponent(NAME_COMP))->m_name = "Exit";
	PhysicsComp* exit_phyx_comp_2_1 = (PhysicsComp*)exit_2_1->AddComponent(PHYSICS_COMP);
	TransformComp* exit_trans_comp_2_1 = (TransformComp*)exit_2_1->AddComponent(TRANSFORM_COMP);
	TriggerComp* exit_trigger_comp_2_1 = (TriggerComp*)exit_2_1->AddComponent(TRIGGER_COMP);
	RenderComp* exit_render_comp_2_1 = (RenderComp*)exit_2_1->AddComponent(RENDER_COMP);

	AddVertsForAABB2D(exit_render_comp_2_1->m_main_group.verts, AABB2(exit_phyx_comp_2_1->m_radius * 2.0f, exit_phyx_comp_2_1->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	exit_render_comp_2_1->m_main_texture = "Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png";

	exit_trigger_comp_2_1->m_portal_active = true;
	exit_trigger_comp_2_1->m_portal_to_position = Vec2(4.0f, 2.0f);

	exit_trigger_comp_2_1->m_transfer_map = true;
	exit_trigger_comp_2_1->m_map_to_transfer = 1;

	exit_trans_comp_2_1->m_transform.m_position = Vec2(2.0f, 2.0f);


	//--------------------------------------------------------------------------
	// Exit map 1-2

	Entity* exit_1_2 = m_maps[1]->m_admin.CreateEntity();
	((NameComp*)exit_1_2->AddComponent(NAME_COMP))->m_name = "Exit";
	PhysicsComp* exit_phyx_comp_1_2 = (PhysicsComp*)exit_1_2->AddComponent(PHYSICS_COMP);
	TransformComp* exit_trans_comp_1_2 = (TransformComp*)exit_1_2->AddComponent(TRANSFORM_COMP);
	TriggerComp* exit_trigger_comp_1_2 = (TriggerComp*)exit_1_2->AddComponent(TRIGGER_COMP);
	RenderComp* exit_render_comp_1_2 = (RenderComp*)exit_1_2->AddComponent(RENDER_COMP);

	AddVertsForAABB2D(exit_render_comp_1_2->m_main_group.verts, AABB2(exit_phyx_comp_1_2->m_radius * 2.0f, exit_phyx_comp_1_2->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	exit_render_comp_1_2->m_main_texture = "Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png";

	exit_trigger_comp_1_2->m_portal_active = false;
	exit_trigger_comp_1_2->m_portal_to_position = Vec2(2.0f, 2.0f);

	exit_trigger_comp_1_2->m_transfer_map = false;
	exit_trigger_comp_1_2->m_map_to_transfer = 2;

	exit_trans_comp_1_2->m_transform.m_position = Vec2(4.0f, 2.0f);

	//--------------------------------------------------------------------------
	// Exit map 1-0

	Entity* exit_1_0 = m_maps[1]->m_admin.CreateEntity();
	((NameComp*)exit_1_0->AddComponent(NAME_COMP))->m_name = "Exit";
	PhysicsComp* exit_phyx_comp_1_0 = (PhysicsComp*)exit_1_0->AddComponent(PHYSICS_COMP);
	TransformComp* exit_trans_comp_1_0 = (TransformComp*)exit_1_0->AddComponent(TRANSFORM_COMP);
	TriggerComp* exit_trigger_comp_1_0 = (TriggerComp*)exit_1_0->AddComponent(TRIGGER_COMP);
	RenderComp* exit_render_comp_1_0 = (RenderComp*)exit_1_0->AddComponent(RENDER_COMP);

	AddVertsForAABB2D(exit_render_comp_1_0->m_main_group.verts, AABB2(exit_phyx_comp_1_0->m_radius * 2.0f, exit_phyx_comp_1_0->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	exit_render_comp_1_0->m_main_texture = "Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png";

	exit_trigger_comp_1_0->m_portal_active = true;
	exit_trigger_comp_1_0->m_portal_to_position = Vec2(8.0f, 8.0f);

	exit_trigger_comp_1_0->m_transfer_map = true;
	exit_trigger_comp_1_0->m_map_to_transfer = 0;

	exit_trans_comp_1_0->m_transform.m_position = Vec2(2.0f, 2.0f);


	//--------------------------------------------------------------------------
	// Exit map 0-1

	Entity* exit_0_1 = m_maps[m_map_id]->m_admin.CreateEntity();
	((NameComp*)exit_0_1->AddComponent(NAME_COMP))->m_name = "Exit";
	PhysicsComp* exit_phyx_comp_0_1 = (PhysicsComp*)exit_0_1->AddComponent(PHYSICS_COMP);
	TransformComp* exit_trans_comp_0_1 = (TransformComp*)exit_0_1->AddComponent(TRANSFORM_COMP);
	TriggerComp* exit_trigger_comp_0_1 = (TriggerComp*)exit_0_1->AddComponent(TRIGGER_COMP);
	RenderComp* exit_render_comp_0_1 = (RenderComp*)exit_0_1->AddComponent(RENDER_COMP);

	AddVertsForAABB2D(exit_render_comp_0_1->m_main_group.verts, AABB2(exit_phyx_comp_0_1->m_radius * 2.0f, exit_phyx_comp_0_1->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	exit_render_comp_0_1->m_main_texture = "Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png";

	exit_trigger_comp_0_1->m_portal_active = true;
	exit_trigger_comp_0_1->m_portal_to_position = Vec2(2.0f, 2.0f);

	exit_trigger_comp_0_1->m_transfer_map = true;
	exit_trigger_comp_0_1->m_map_to_transfer = 1;

	exit_trans_comp_0_1->m_transform.m_position = Vec2(8.0f, 8.0f);

	//--------------------------------------------------------------------------
	// Player
	Entity* player = m_maps[m_map_id]->m_admin.CreateEntity();

	NameComp* player_name_comp = (NameComp*)player->AddComponent( NAME_COMP );
	player_name_comp->m_name = "player";

	PhysicsComp* player_physics_comp = (PhysicsComp*)player->AddComponent( PHYSICS_COMP );
	player_physics_comp->m_static_object = false;

	CameraComp* player_camera_comp = (CameraComp*)player->AddComponent( CAMERA_COMP );
	player_camera_comp->m_active = true;
	Vec2 camera_dims = g_gameConfigBlackboard.GetValue( "camera_dims", Vec2::ONE );
	AABB2 screen( camera_dims.x, camera_dims.y );
	g_gameConfigBlackboard.SetValue( "screen", ToString( screen ) );
	player_camera_comp->SetOrthProjMat( screen.GetBottomLeft(), screen.GetTopRight() );

	player->AddComponent( INTENT_COMP );
	player->AddComponent( INPUT_COMP );
	player->AddComponent( INTERACT_COMP );
	player->AddComponent( QUEST_CARRIER_COMP );

	StatsComp* player_stats = (StatsComp*) player->AddComponent( STATS_COMP );
	player_stats->m_health = 40.0f;

	TransformComp* player_transform_comp = (TransformComp*)player->AddComponent( TRANSFORM_COMP );
	player_transform_comp->m_transform.m_position = Vec2( 4.5, 2.5 );

	RenderComp* render_comp_player = (RenderComp*)player->AddComponent( RENDER_COMP );
	AddVertsForAABB2D( render_comp_player->m_main_group.verts, AABB2( player_physics_comp->m_radius * 2.0f, player_physics_comp->m_radius * 2.0f ), Rgba::WHITE, Vec2( 0.0f, 0.75f ), Vec2( 0.25f, 1.0f ) );
	render_comp_player->m_main_texture = "Data/Images/DawnLike/Mage.png";

	//--------------------------------------------------------------------------
	// Quest
	Entity* kill_enemies_quest = m_maps[m_map_id]->m_admin.CreateEntity();
	QuestComp* quest_comp = (QuestComp*)kill_enemies_quest->AddComponent( QUEST_COMP );
	quest_comp->accept_text = "Thanks, for this. Please hurry...";
	quest_comp->init_text = "....Please....Please help.... There's so many creatures...";
	quest_comp->complete_text = "THANK YOU SO MUCH!";
	quest_comp->fail_text = "...";
	quest_comp->quest_name = "Help Skittish Joe";
	quest_comp->num_enemies_to_kill = 3;
	quest_comp->is_active = true;
	quest_comp->open_exit_on_finished = exit_1_2->m_id;
	quest_comp->map_exit_is_resides = 1;

	//--------------------------------------------------------------------------
	// NPC
	Entity* silent_joe = m_maps[m_map_id]->m_admin.CreateEntity();
	silent_joe->AddComponent( INTERACT_COMP );
	silent_joe->AddComponent( AI_COMP );
	QuestGiverComp* npc_quest_giver_comp = (QuestGiverComp*)silent_joe->AddComponent( QUEST_GIVER_COMP );
	npc_quest_giver_comp->AddQuest( quest_comp );

	NameComp* npc_name_comp = (NameComp*)silent_joe->AddComponent( NAME_COMP );
	TransformComp* npc_transform_comp = (TransformComp*)silent_joe->AddComponent( TRANSFORM_COMP );
	
	PhysicsComp* npc_physics_comp = (PhysicsComp*) silent_joe->AddComponent( PHYSICS_COMP );

	RenderComp* npc_render_comp = (RenderComp*) silent_joe->AddComponent( RENDER_COMP );
	AddVertsForAABB2D(npc_render_comp->m_main_group.verts, AABB2(npc_physics_comp->m_radius * 2.0f, npc_physics_comp->m_radius * 2.0f), Rgba::WHITE, Vec2( .75f, 0.9333f ), Vec2(0.875f, 1.0f) );
	npc_render_comp->m_main_texture = "Data/Images/DawnLike/Player0.png";

	npc_name_comp->m_name = "Help Skittish Joe";
	npc_transform_comp->m_transform.m_position = Vec2( 6.0f, 3.0f );

	// Enemy graphic
	SpriteSheet sheet((TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile("Data/Images/DawnLike/Avian0.png"), IntVec2(8, 13));
	const SpriteDefinition& def = sheet.GetSpriteDefinition(3, 9);
	def.GetUVs(bot_left_uvs, top_right_uvs);

	//--------------------------------------------------------------------------
	// Enemy 3
	Entity* enemy_3 = m_maps[1]->m_admin.CreateEntity();
	NameComp* enemy_3_name_comp = (NameComp*)enemy_3->AddComponent(NAME_COMP);
	AIComp* enemy_3_ai_comp = (AIComp*)enemy_3->AddComponent(AI_COMP);
	TransformComp* enemy_3_transform_comp = (TransformComp*)enemy_3->AddComponent(TRANSFORM_COMP);

	RenderComp* enemy_3_render_comp = (RenderComp*)enemy_3->AddComponent(RENDER_COMP);
	PhysicsComp* enemy_3_physics_comp = (PhysicsComp*)enemy_3->AddComponent(PHYSICS_COMP);

	enemy_3_physics_comp->m_max_speed = 0.4f;
	enemy_3_physics_comp->m_static_object = false;

	enemy_3->AddComponent( INTENT_COMP );
	((StatsComp*)enemy_3->AddComponent( STATS_COMP ))->m_team = ENEMY_TEAM;

	AddVertsForAABB2D(enemy_3_render_comp->m_main_group.verts, AABB2(enemy_3_physics_comp->m_radius * 2.0f, enemy_3_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	enemy_3_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_3_name_comp->m_name = "Enemy_2";
	enemy_3_transform_comp->m_transform.m_position = Vec2(2.0f, 7.41f);
	enemy_3_ai_comp->m_vision_radius = 2.0f;

	//--------------------------------------------------------------------------
	// Enemy 2
	Entity* enemy_2 = m_maps[1]->m_admin.CreateEntity();
	NameComp* enemy_2_name_comp = (NameComp*)enemy_2->AddComponent(NAME_COMP);
	AIComp* enemy_2_ai_comp = (AIComp*)enemy_2->AddComponent(AI_COMP);
	TransformComp* enemy_2_transform_comp = (TransformComp*)enemy_2->AddComponent(TRANSFORM_COMP);

	RenderComp* enemy_2_render_comp = (RenderComp*)enemy_2->AddComponent(RENDER_COMP);
	PhysicsComp* enemy_2_physics_comp = (PhysicsComp*)enemy_2->AddComponent(PHYSICS_COMP);

	enemy_2_physics_comp->m_max_speed = 0.4f;
	enemy_2_physics_comp->m_static_object = false;

	enemy_2->AddComponent( INTENT_COMP );
	((StatsComp*)enemy_2->AddComponent(STATS_COMP))->m_team = ENEMY_TEAM;

	AddVertsForAABB2D(enemy_2_render_comp->m_main_group.verts, AABB2(enemy_2_physics_comp->m_radius * 2.0f, enemy_2_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	enemy_2_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_2_name_comp->m_name = "Enemy_2";
	enemy_2_transform_comp->m_transform.m_position = Vec2(2.59f, 8.0f);
	enemy_2_ai_comp->m_vision_radius = 2.0f;

	//--------------------------------------------------------------------------
	// Enemy 1
	Entity* enemy_1 = m_maps[1]->m_admin.CreateEntity();
	NameComp* enemy_1_name_comp = (NameComp*)enemy_1->AddComponent( NAME_COMP );
	AIComp* enemy_1_ai_comp = (AIComp*) enemy_1->AddComponent( AI_COMP );
	TransformComp* enemy_1_transform_comp = (TransformComp*) enemy_1->AddComponent( TRANSFORM_COMP );

	RenderComp* enemy_1_render_comp = (RenderComp*)enemy_1->AddComponent( RENDER_COMP );
	PhysicsComp* enemy_1_physics_comp = (PhysicsComp*)enemy_1->AddComponent( PHYSICS_COMP );

	enemy_1_physics_comp->m_max_speed = 0.4f;
	enemy_1_physics_comp->m_static_object = false;

	enemy_1->AddComponent( INTENT_COMP );
	((StatsComp*)enemy_1->AddComponent(STATS_COMP))->m_team = ENEMY_TEAM;

	AddVertsForAABB2D(enemy_1_render_comp->m_main_group.verts, AABB2(enemy_1_physics_comp->m_radius * 2.0f, enemy_1_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs );
	enemy_1_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_1_name_comp->m_name = "Enemy_1";
	enemy_1_transform_comp->m_transform.m_position = Vec2( 2.0f, 8.0f );
	enemy_1_ai_comp->m_vision_radius = 2.0f;

	
}

//--------------------------------------------------------------------------
/**
* Shutdown
*/
void Game::Shutdown()
{
	TileDefinition::DeconstructDefinitions();

	for (auto& map_pair : m_maps)
	{
		SAFE_DELETE(map_pair.second);
	}

	for (auto& system : EntityAdmin::GetMaster()->m_systems)
	{
		SAFE_DELETE(system);
	}

	delete EntityAdmin::GetMaster();
}

static int g_index = 0;

//--------------------------------------------------------------------------
/**
* HandleKeyPressed
*/
bool Game::HandleKeyPressed( unsigned char keyCode )
{
	UNUSED(keyCode);
	if( keyCode == 'O' )
	{
		m_inDebug = !m_inDebug;
	}
	return false;
}

//--------------------------------------------------------------------------
/**
* HandleKeyReleased
*/
bool Game::HandleKeyReleased( unsigned char keyCode )
{
	UNUSED(keyCode);
	return false;
}

//--------------------------------------------------------------------------
/**
* GameRender
*/
void Game::GameRender() const
{
	g_theRenderer->BindShader( m_shader );
	g_theRenderer->BindSampler( SAMPLE_MODE_POINT );

	m_maps.at(m_map_id)->Render();

	g_theDebugRenderSystem->RenderToCamera( &m_CurentCamera );
}

//--------------------------------------------------------------------------
/**
* UpdateGame
*/
void Game::UpdateGame( float deltaSeconds )
{
	UpdateCamera( deltaSeconds );

	HandleMapTransfers();

	m_maps.at( m_map_id )->Update( deltaSeconds );
}


//--------------------------------------------------------------------------
/**
* GetCurrentMap
*/
Map* Game::GetCurrentMap() const
{
	return m_maps.at( m_map_id );
}

//--------------------------------------------------------------------------
/**
* GetMap
*/
Map* Game::GetMap( int id ) const
{
	return m_maps.at( id );
}

//--------------------------------------------------------------------------
/**
* UpdateCamera
*/
void Game::UpdateCamera( float deltaSeconds )
{
	UNUSED( deltaSeconds );
	m_CurentCamera.SetModelMatrix( Matrix44::IDENTITY );
	m_CurentCamera.SetColorTargetView( g_theRenderer->GetColorTargetView() );
	m_CurentCamera.SetDepthTargetView( g_theRenderer->GetDepthTargetView() );
	g_theRenderer->BeginCamera( &m_CurentCamera );
}

//--------------------------------------------------------------------------
/**
* HandleMapTransfers
*/
void Game::HandleMapTransfers()
{
	int switch_cur_map = -1;

	for( Entity& entity : GetCurrentMap()->m_admin.m_entities )
	{
		if( entity.m_map_to_transfer_to != -1 )
		{
			InputComp* player_input = (InputComp*) entity.GetComponent( INPUT_COMP );
			if( player_input )
			{
				switch_cur_map = entity.m_map_to_transfer_to;
			}

			Map* cur_map = GetCurrentMap();
			Map* switch_to_map = GetMap( entity.m_map_to_transfer_to );

			// Do this BEFORE adding and removing entities
			entity.m_map_to_transfer_to = -1;

			switch_to_map->m_admin.AddEntity( entity );
			cur_map->m_admin.RemoveEntity( entity.m_id );
		}
	}

	if( switch_cur_map != -1 )
	{
		m_map_id = switch_cur_map;
	}
}
