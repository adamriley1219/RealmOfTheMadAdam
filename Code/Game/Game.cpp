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

	IntVec2 map_1_dims( 50, 50 );

	m_map_id = 0;
	m_maps[0] = new Map( 0, IntVec2( 10, 10 ), m_terrain_sheet, TileType::GRASS_TILE, TileType::STONE_TILE );
	m_maps[1] = new Map( 1, map_1_dims, m_terrain_sheet, TileType::SAND_TILE, TileType::STONE_TILE );
	m_maps[2] = new Map( 2, IntVec2( 5, 7 ), m_terrain_sheet, TileType::MUD_TILE, TileType::STONE_TILE );

	Vec2 bot_left_uvs;
	Vec2 top_right_uvs;
	std::string pixel_art_vehicles = "Data/Images/DawnLike/16x16-fantasy-pixel-art-vehicles.png";
	std::string pixel_NPC= "Data/Images/DawnLike/Avian0.png";

	SpriteSheet exit_sheet((TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile( pixel_art_vehicles.c_str() ), IntVec2(18, 4));
	const SpriteDefinition& exit_sdef = exit_sheet.GetSpriteDefinition(4, 1);

	SpriteSheet NPC_sheet((TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile( pixel_NPC.c_str()), IntVec2(8, 13) );
	const SpriteDefinition& enemy_def = NPC_sheet.GetSpriteDefinition(3, 9);
	
	//--------------------------------------------------------------------------
	exit_sdef.GetUVs(bot_left_uvs, top_right_uvs);
	//--------------------------------------------------------------------------
	// Exit map 2-1
	CreateExit( Vec2(2.0f, 2.0f), Vec2(4.0f, 2.0f), 2, 1, true, pixel_art_vehicles, bot_left_uvs, top_right_uvs );

	//--------------------------------------------------------------------------
	// Exit map 1-2
	Entity* exit_1_2 = CreateExit( Vec2(4.0f, 2.0f), Vec2(2.0f, 2.0f), 1, 2, false, pixel_art_vehicles, bot_left_uvs, top_right_uvs );

	//--------------------------------------------------------------------------
	// Exit map 1-0
	CreateExit( Vec2(2.0f, 2.0f), Vec2(8.0f, 8.0f), 1, 0, true, pixel_art_vehicles, bot_left_uvs, top_right_uvs );

	//--------------------------------------------------------------------------
	// Exit map 0-1
	CreateExit( Vec2(8.0f, 8.0f), Vec2(2.0f, 2.0f), m_map_id, 1, true, pixel_art_vehicles, bot_left_uvs, top_right_uvs );

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
	player_stats->m_damage_multiplier = 4.0f;

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

	//--------------------------------------------------------------------------
	enemy_def.GetUVs(bot_left_uvs, top_right_uvs);
	float move_speed = 0.4f;
	float vision_distance = 1.0f;
	float damage_multiplier = 0.0f;
	//--------------------------------------------------------------------------
	// Enemy 3
	CreateEnemy( "Enemy_3", Vec2(2.0f, 7.41f), 1, damage_multiplier, move_speed, vision_distance, pixel_NPC, bot_left_uvs, top_right_uvs );

	//--------------------------------------------------------------------------
	// Enemy 2
	CreateEnemy( "Enemy_2", Vec2(2.59f, 8.0f), 1, damage_multiplier, move_speed, vision_distance, pixel_NPC, bot_left_uvs, top_right_uvs );

	//--------------------------------------------------------------------------
	// Enemy 1
	CreateEnemy( "Enemy_1", Vec2( 2.0f, 8.0f ), 1, damage_multiplier, move_speed, vision_distance, pixel_NPC, bot_left_uvs, top_right_uvs );

	Vec2 spawn_area( map_1_dims.x - 2.0f, map_1_dims.y - 2.0f );
	for( float draw_at_x = 3.0f; draw_at_x < spawn_area.x; draw_at_x += 2.0f )
	{
		for( float draw_at_y = 3.0f; draw_at_y < spawn_area.y; draw_at_y += 2.0f )
		{
			CreateEnemy( "Enemy", Vec2( draw_at_x, draw_at_y ), 1, damage_multiplier, move_speed, vision_distance, pixel_NPC, bot_left_uvs, top_right_uvs );
		}
	}

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
* CreateExit
*/
Entity* Game::CreateExit( const Vec2& portal_from, const Vec2& portal_transfer_to, int from_map, int to_map, bool active, const std::string& texture, const Vec2& bot_left_uvs, const Vec2& top_right_uvs )
{
	Entity* exit = m_maps[from_map]->m_admin.CreateEntity();
	((NameComp*)exit->AddComponent(NAME_COMP))->m_name = "Exit";
	PhysicsComp* exit_phyx_comp = (PhysicsComp*)exit->AddComponent(PHYSICS_COMP);
	TransformComp* exit_trans_comp = (TransformComp*)exit->AddComponent(TRANSFORM_COMP);
	TriggerComp* exit_trigger_comp = (TriggerComp*)exit->AddComponent(TRIGGER_COMP);
	RenderComp* exit_render_comp = (RenderComp*)exit->AddComponent(RENDER_COMP);

	AddVertsForAABB2D( exit_render_comp->m_main_group.verts, AABB2(exit_phyx_comp->m_radius * 2.0f, exit_phyx_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs );
	exit_render_comp->m_main_texture = texture;

	exit_trigger_comp->m_portal_active = active;
	exit_trigger_comp->m_portal_to_position = portal_transfer_to;

	exit_trigger_comp->m_transfer_map = active;
	exit_trigger_comp->m_map_to_transfer = to_map;

	exit_trans_comp->m_transform.m_position = portal_from;
	return exit;
}

//--------------------------------------------------------------------------
/**
* CreateEnemy
*/
Entity* Game::CreateEnemy(const std::string& name, const Vec2& position, int map_idx, float damage_multiplier, float max_speed, float vision_radius, const std::string& texture, const Vec2& bot_left_uvs, const Vec2& top_right_uvs)
{
	Entity* enemy = m_maps[map_idx]->m_admin.CreateEntity();
	NameComp* enemy_name_comp = (NameComp*)enemy->AddComponent(NAME_COMP);
	AIComp* enemy_ai_comp = (AIComp*)enemy->AddComponent(AI_COMP);
	TransformComp* enemy_transform_comp = (TransformComp*)enemy->AddComponent(TRANSFORM_COMP);

	RenderComp* enemy_render_comp = (RenderComp*)enemy->AddComponent(RENDER_COMP);
	PhysicsComp* enemy_physics_comp = (PhysicsComp*)enemy->AddComponent(PHYSICS_COMP);

	enemy_physics_comp->m_max_speed = max_speed;
	enemy_physics_comp->m_static_object = false;

	enemy->AddComponent(INTENT_COMP);
	StatsComp* stats = ((StatsComp*)enemy->AddComponent(STATS_COMP));
	stats->m_team = ENEMY_TEAM;
	stats->m_damage_multiplier = damage_multiplier;

	AddVertsForAABB2D(enemy_render_comp->m_main_group.verts, AABB2(enemy_physics_comp->m_radius * 2.0f, enemy_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	enemy_render_comp->m_main_texture = texture;

	enemy_name_comp->m_name = name;
	enemy_transform_comp->m_transform.m_position = position;
	enemy_ai_comp->m_vision_radius = vision_radius;

	return enemy;
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
