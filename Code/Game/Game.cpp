#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/Debug/DebugRenderSystem.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"

#include "Engine/Core/Debug/DevConsole.hpp"
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

	EntityAdmin::m_master.m_systems.push_back( new GameInpuSystem() );
	EntityAdmin::m_master.m_systems.push_back( new GamePhysicsSystem() );
	EntityAdmin::m_master.m_systems.push_back( new CombatSystem() );
	EntityAdmin::m_master.m_systems.push_back( new QuestSystem() );
	EntityAdmin::m_master.m_systems.push_back( new TriggerSystem() );
	EntityAdmin::m_master.m_systems.push_back( new UISystem() );
	EntityAdmin::m_master.m_systems.push_back( new RenderSystem() );
	EntityAdmin::m_master.m_systems.push_back( new MovementSystem() );

	m_map_id = 0;
	m_maps[m_map_id] = new Map( m_map_id, IntVec2( 10, 30 ), m_terrain_sheet, TileType::GRASS_TILE, TileType::STONE_TILE );

	Entity* player = m_maps[m_map_id]->m_admin.CreateEntity();
	player->AddComponent( new NameComp( "player" ) );
	player->AddComponent( new PhysicsComp( ) );
	player->AddComponent( new RenderComp( ) );
	player->AddComponent( new TransformComp( 4.5, 2.5 ) );
	m_player_id = player->m_id;

	// Add singletons
	EntityAdmin::m_master.AddComponent( new InputComp() );
}

//--------------------------------------------------------------------------
/**
* Shutdown
*/
void Game::Shutdown()
{
	TileDefinition::DeconstructDefinitions();

	// TODO: Delete maps 

	for( System*& sys : EntityAdmin::m_master.m_systems )
	{
		SAFE_DELETE( sys );
	}
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
	m_maps.at(m_map_id)->Update(deltaSeconds);

	UpdateCamera( deltaSeconds );
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
* IsPlayer
*/
bool Game::IsPlayer( EntityID id )
{
	return ( m_player_id == id );
}

//--------------------------------------------------------------------------
/**
* ImGUIWidget
*/
void Game::ImGUIWidget()
{
	ImGui::Begin("TestWindow");

	ImGui::Text(Stringf("%s", "100% in there.").c_str());

	bool testCheck1 = true;
	bool testCheck2 = false;
	ImGui::Checkbox("test check box 1", &testCheck1);
	ImGui::Checkbox("test check box 2", &testCheck2);

	if (testCheck2)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "Changed test check 2");
	}
	if (!testCheck1)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "Changed test check 1");
	}

	float beginning = 50.0f;
	ImGui::SliderFloat("slidy", &beginning, 0, 100);

	if (beginning != 50.0f)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "slidy changed");
	}

	float col[3] = { 1.0f, 1.0f, 1.0f };
	ImGui::ColorEdit3("color thing?", col);

	ImGui::End();
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
