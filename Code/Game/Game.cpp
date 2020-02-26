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

#include "Game/Components/InputComp.hpp"
#include "Game/Components/NameComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Components/RenderComp.hpp"
#include "Game/Components/TransformComp.hpp"
#include "Game/Components/UIComp.hpp"
#include "Game/Components/QuestGiverComp.hpp"
#include "Game/Components/QuestCarrierComp.hpp"
#include "Game/Components/IntentComp.hpp"
#include "Game/Components/CameraComp.hpp"
#include "Game/Components/InteractComp.hpp"
#include "Game/Components/AIComp.hpp"


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
	EntityAdmin::GetMaster()->m_systems.push_back( new GamePhysicsSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new CombatSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new QuestSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new TriggerSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new UISystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new RenderSystem() );
	EntityAdmin::GetMaster()->m_systems.push_back( new MovementSystem() );

	m_map_id = 0;
	m_maps[m_map_id] = new Map( m_map_id, IntVec2( 10, 10 ), m_terrain_sheet, TileType::GRASS_TILE, TileType::STONE_TILE );

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
	player_camera_comp->m_camera.SetOrthographicProjection( screen.GetBottomLeft(), screen.GetTopRight() );

	player->AddComponent( INTENT_COMP );
	player->AddComponent( INPUT_COMP );
	player->AddComponent( INTERACT_COMP );
	player->AddComponent( QUEST_CARRIER_COMP );

	TransformComp* player_transform_comp = (TransformComp*)player->AddComponent( TRANSFORM_COMP );
	player_transform_comp->m_transform.m_position = Vec2( 4.5, 2.5 );

	RenderComp* render_comp_player = (RenderComp*)player->AddComponent( RENDER_COMP );
	AddVertsForAABB2D( render_comp_player->m_main_group.verts, AABB2( player_physics_comp->m_radius * 2.0f, player_physics_comp->m_radius * 2.0f ), Rgba::WHITE, Vec2( 0.0f, 0.75f ), Vec2( 0.25f, 1.0f ) );
	render_comp_player->m_main_texture = "Data/Images/DawnLike/Mage.png";

	//--------------------------------------------------------------------------
	// NPC
	Entity* silent_joe = m_maps[m_map_id]->m_admin.CreateEntity();
	silent_joe->AddComponent( INTERACT_COMP );
	silent_joe->AddComponent( AI_COMP );
	QuestGiverComp* npc_quest_giver_comp = (QuestGiverComp*)silent_joe->AddComponent( QUEST_GIVER_COMP );
	npc_quest_giver_comp->accept_text = "Thanks, for this. Please hurry...";
	npc_quest_giver_comp->init_text = "....Please....Please help.... There's so many creatures...";
	npc_quest_giver_comp->complete_text = "THANK YOU SO MUCH!";
	npc_quest_giver_comp->fail_text = "...";
	npc_quest_giver_comp->quest_name = "Kill the creatures.";

	NameComp* npc_name_comp = (NameComp*)silent_joe->AddComponent( NAME_COMP );
	TransformComp* npc_transform_comp = (TransformComp*)silent_joe->AddComponent( TRANSFORM_COMP );
	
	PhysicsComp* npc_physics_comp = (PhysicsComp*) silent_joe->AddComponent( PHYSICS_COMP );

	RenderComp* npc_render_comp = (RenderComp*) silent_joe->AddComponent( RENDER_COMP );
	AddVertsForAABB2D(npc_render_comp->m_main_group.verts, AABB2(npc_physics_comp->m_radius * 2.0f, npc_physics_comp->m_radius * 2.0f), Rgba::WHITE, Vec2( .75f, 0.9333f ), Vec2(0.875f, 1.0f) );
	npc_render_comp->m_main_texture = "Data/Images/DawnLike/Player0.png";

	npc_name_comp->m_name = "Silent Joe";
	npc_transform_comp->m_transform.m_position = Vec2( 6.0f, 3.0f );

	// Enemy graphic
	SpriteSheet sheet((TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile("Data/Images/DawnLike/Avian0.png"), IntVec2(8, 13));
	const SpriteDefinition& def = sheet.GetSpriteDefinition(3, 9);
	Vec2 bot_left_uvs;
	Vec2 top_right_uvs;
	def.GetUVs(bot_left_uvs, top_right_uvs);

	//--------------------------------------------------------------------------
	// Enemy 3
	Entity* enemy_3 = m_maps[m_map_id]->m_admin.CreateEntity();
	NameComp* enemy_3_name_comp = (NameComp*)enemy_3->AddComponent(NAME_COMP);
	AIComp* enemy_3_ai_comp = (AIComp*)enemy_3->AddComponent(AI_COMP);
	TransformComp* enemy_3_transform_comp = (TransformComp*)enemy_3->AddComponent(TRANSFORM_COMP);

	RenderComp* enemy_3_render_comp = (RenderComp*)enemy_3->AddComponent(RENDER_COMP);
	PhysicsComp* enemy_3_physics_comp = (PhysicsComp*)enemy_3->AddComponent(PHYSICS_COMP);

	enemy_3->AddComponent( INTENT_COMP );

	AddVertsForAABB2D(enemy_3_render_comp->m_main_group.verts, AABB2(enemy_3_physics_comp->m_radius * 2.0f, enemy_3_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	enemy_3_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_3_name_comp->m_name = "Enemy_2";
	enemy_3_transform_comp->m_transform.m_position = Vec2(2.0f, 7.41f);
	enemy_3_ai_comp->m_trigger_range = 2.0f;

	//--------------------------------------------------------------------------
	// Enemy 2
	Entity* enemy_2 = m_maps[m_map_id]->m_admin.CreateEntity();
	NameComp* enemy_2_name_comp = (NameComp*)enemy_2->AddComponent(NAME_COMP);
	AIComp* enemy_2_ai_comp = (AIComp*)enemy_2->AddComponent(AI_COMP);
	TransformComp* enemy_2_transform_comp = (TransformComp*)enemy_2->AddComponent(TRANSFORM_COMP);

	RenderComp* enemy_2_render_comp = (RenderComp*)enemy_2->AddComponent(RENDER_COMP);
	PhysicsComp* enemy_2_physics_comp = (PhysicsComp*)enemy_2->AddComponent(PHYSICS_COMP);

	enemy_2->AddComponent( INTENT_COMP );

	AddVertsForAABB2D(enemy_2_render_comp->m_main_group.verts, AABB2(enemy_2_physics_comp->m_radius * 2.0f, enemy_2_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs);
	enemy_2_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_2_name_comp->m_name = "Enemy_2";
	enemy_2_transform_comp->m_transform.m_position = Vec2(2.59f, 8.0f);
	enemy_2_ai_comp->m_trigger_range = 2.0f;

	//--------------------------------------------------------------------------
	// Enemy 1
	Entity* enemy_1 = m_maps[m_map_id]->m_admin.CreateEntity();
	NameComp* enemy_1_name_comp = (NameComp*)enemy_1->AddComponent( NAME_COMP );
	AIComp* enemy_1_ai_comp = (AIComp*) enemy_1->AddComponent( AI_COMP );
	TransformComp* enemy_1_transform_comp = (TransformComp*) enemy_1->AddComponent( TRANSFORM_COMP );

	RenderComp* enemy_1_render_comp = (RenderComp*)enemy_1->AddComponent( RENDER_COMP );
	PhysicsComp* enemy_1_physics_comp = (PhysicsComp*)enemy_1->AddComponent( PHYSICS_COMP );

	enemy_1->AddComponent( INTENT_COMP );

	AddVertsForAABB2D(enemy_1_render_comp->m_main_group.verts, AABB2(enemy_1_physics_comp->m_radius * 2.0f, enemy_1_physics_comp->m_radius * 2.0f), Rgba::WHITE, bot_left_uvs, top_right_uvs );
	enemy_1_render_comp->m_main_texture = "Data/Images/DawnLike/Avian0.png";

	enemy_1_name_comp->m_name = "Enemy_1";
	enemy_1_transform_comp->m_transform.m_position = Vec2( 2.0f, 8.0f );
	enemy_1_ai_comp->m_trigger_range = 2.0f;
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
	m_maps.at(m_map_id)->Update(deltaSeconds);

}


//--------------------------------------------------------------------------
/**
* GetCurrentMap
*/
Map* Game::GetCurrentMap() const
{
	return m_maps.at(m_map_id);
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
