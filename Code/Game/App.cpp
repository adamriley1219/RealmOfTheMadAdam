#include "Game/App.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Debug/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Engine/Memory/Debug/Log.hpp"
#include "Engine/Memory/Debug/Profiler.hpp"
#include "Engine/ImGUI/ImGUI_Interfaces/ProfilerDisplay.hpp"
#include "Engine/Core/Time/Clock.hpp"

#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/Debug/DebugRenderSystem.hpp"
#include "Engine/Renderer/BitmapFont.hpp"

#include "Engine/Math/RNG.hpp"

#include "Engine/ImGUI/ImGUISystem.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Physics/PhysicsSystem.hpp"

#include "Engine/Core/Strings/NamedStrings.hpp"
#include "Engine/Core/Strings/StringUtils.hpp"

#include "Engine/Memory/Debug/LogProfile.hpp"

#include "Game/GameCommon.hpp"

//--------------------------------------------------------------------------
// Global Singletons
//--------------------------------------------------------------------------
RenderContext* g_theRenderer = nullptr;		// Created and owned by the App
InputSystem* g_theInputSystem = nullptr;
AudioSystem* g_theAudioSystem = nullptr;
App* g_theApp = nullptr;					// Created and owned by Main_Windows.cpp
bool g_isInDebug = false;
RNG* g_theRNG = nullptr;
PhysicsSystem* g_thePhysicsSystem = nullptr;
Game* g_theGame = nullptr;
WindowContext* g_theWindowContext = nullptr;
ImGUISystem* g_theImGUISystem = nullptr;

//--------------------------------------------------------------------------
/**
* Startup
*/
void App::Startup()
{
	g_theRNG = new RNG();
	g_theEventSystem = new EventSystem();
	g_theConsole = new DevConsole( "SquirrelFixedFont" );
	g_theRenderer = new RenderContext( g_theWindowContext );
	g_theDebugRenderSystem = new DebugRenderSystem(g_theRenderer, 50.0f, 100.0f, "SquirrelFixedFont");
	g_theInputSystem = new InputSystem();
	g_theAudioSystem = new AudioSystem();
	g_thePhysicsSystem = new PhysicsSystem();
	g_theImGUISystem = new ImGUISystem( g_theRenderer );

	g_theGame = new Game();

	LogSystemStartup( "Data/Log/Log.txt" );
	ProfilerSystemInit();

	ClockSystemStartup();
	m_gameClock = new Clock(&Clock::Master);

	g_theEventSystem->Startup();
	g_theRenderer->Startup();
	g_theDebugRenderSystem->Startup();
	g_theConsole->Startup();
	g_thePhysicsSystem->Startup();
	g_theImGUISystem->Startup();

	g_theGame->Startup();

	RegisterEvents();
}

//--------------------------------------------------------------------------
/**
* Shutdown
*/
void App::Shutdown()
{
	g_theGame->Shutdown();

	g_theImGUISystem->Shutdown();
	g_thePhysicsSystem->Shutdown();
	g_theConsole->Shutdown();
	g_theDebugRenderSystem->Shutdown();
	g_theRenderer->Shutdown();
	g_theEventSystem->Shutdown();

	ProfilerSystemDeinit();
	LogProfilePrintToLog();
	LogSystemShutdown();

	SAFE_DELETE( g_theGame );

	SAFE_DELETE( m_gameClock );
	SAFE_DELETE( g_theImGUISystem );
	SAFE_DELETE( g_theAudioSystem );
	SAFE_DELETE( g_theInputSystem );
	SAFE_DELETE( g_theConsole );
	SAFE_DELETE( g_theDebugRenderSystem );
	SAFE_DELETE( g_theRenderer );
	SAFE_DELETE( g_theRNG );
}

//--------------------------------------------------------------------------
/**
* RunFrame
*/
void App::RunFrame()
{
	if (m_isSlowMo)
	{
		m_gameClock->Dilate(0.1f);
	}
	else if (m_isFastMo)
	{
		m_gameClock->Dilate(4.0f);
	}
	else
	{
		m_gameClock->Dilate(1.0f);
	}


	BeginFrame();
	Update((float)m_gameClock->GetFrameTime());
	Render();
	EndFrame();
}

//--------------------------------------------------------------------------
/**
* HandleKeyPressed
*/
bool App::HandleKeyPressed( unsigned char keyCode )
{
	if( g_theConsole->HandleKeyPress( keyCode ) )
	{
		return true;
	}
	switch( keyCode )
	{
	case 192: // '~' press

		break;
	case 112: // F1
		ProfilerToggleDisply();
		ToggleDebug();
		return true;
		break;
	case 113: // F2
		ProfilerTogglePaused();
		return true;
		break;
	case 114: // F2
		ProfilerToggleTreeMode();
		return true;
		break;
	case 'T':
		m_isSlowMo = true;
		return true;
		break;
	case 'Y':
		m_isFastMo = true;
		return true;
		break;
	case 'w': // F8 press
		delete g_theGame;
		g_theGame = new Game();
		return true;
		break;
	default:
		return g_theGame->HandleKeyPressed( keyCode );
		break;
	}
	return false;
}

//--------------------------------------------------------------------------
/**
* HandleCharPressed
*/
bool App::HandleCharPressed( unsigned char keyCode )
{
	if( g_theConsole->HandleCharPress( keyCode ) )
	{
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
/**
* HandleKeyReleased
*/
bool App::HandleKeyReleased( unsigned char keyCode )
{
	if( g_theConsole->HandleKeyReleased( keyCode ) )
	{
		return true;
	}
	switch( keyCode )
	{
	case 'T':
		m_isSlowMo = false;
		break;
	case 'Y':
		m_isFastMo = false;
		break;
	case 'H':
		g_theEventSystem->FireEvent( "help" );
		break;
	default:
		return g_theGame->HandleKeyReleased( keyCode );
		break;
	}
	return true;
}

//--------------------------------------------------------------------------
/**
* HandleQuitRequested
*/
bool App::HandleQuitRequested()
{
	m_isQuitting = true;
	return true;
}

//--------------------------------------------------------------------------
/**
* QuitEvent
*/
bool App::QuitEvent( EventArgs& args )
{
	UNUSED( args );
	g_theApp->HandleQuitRequested();
	return true;
}
//--------------------------------------------------------------------------
/**
* IsPaused
*/
bool App::IsPaused() const
{
	return m_gameClock->IsPaused();
}

//--------------------------------------------------------------------------
/**
* Unpause
*/
void App::Unpause()
{
	m_gameClock->Resume();
}

//--------------------------------------------------------------------------
/**
* Pause
*/
void App::Pause()
{
	m_gameClock->Pause();
}


//--------------------------------------------------------------------------
/**
* BeginFrame
*/
void App::BeginFrame()
{
	ClockSystemBeginFrame();
	g_theImGUISystem->		BeginFrame();
	g_theEventSystem->		BeginFrame();
	g_theRenderer->			BeginFrame();
	g_theConsole->			BeginFrame();
	g_theInputSystem->		BeginFrame();
	g_theAudioSystem->		BeginFrame();
	g_theDebugRenderSystem->BeginFrame();
}


//--------------------------------------------------------------------------
/**
* Update
*/
void App::Update( float deltaSeconds )
{
	g_theConsole->			Update();
	g_theGame->				UpdateGame( deltaSeconds );
	g_theDebugRenderSystem->Update();
}

//--------------------------------------------------------------------------
/**
* RenderDebugRenderDebugLeftJoystick
*/
void App::RenderDebugLeftJoystick() const
{
	float inRangex = 2.0f;
	float inRangey = 2.0f;
	float outerRadius = 8.0f;
	float posRadius = 0.5f;
	const XboxController& curController = g_theInputSystem->GetControllerByID(0);
	if( !curController.IsConnected() )
		return;
	const AnalogJoystick& curLJoystick = curController.GetLeftJoystick();
	const Vec2& upRightRef = g_theGame->m_DevColsoleCamera.GetOrthoTopRight();

	Vec3 center
	(	
		upRightRef.x - inRangex - outerRadius
		,	upRightRef.y - inRangey - outerRadius
		,	0.0f
	);

	Vertex_PCU centerVert( center, Rgba( 0.4f, 0.4f, 0.4f, 0.5f ), Vec2( 0.0f, 0.0f ) );
	DrawDisc( centerVert , outerRadius );



	centerVert.color.r = 0.1f;
	centerVert.color.g = 0.1f;
	centerVert.color.b = 0.1f;
	DrawDisc( centerVert , outerRadius * curLJoystick.GetOuterDeadZoneFraction() );
	centerVert.color.r = 0.3f;
	centerVert.color.g = 0.3f;
	centerVert.color.b = 0.3f;
	DrawDisc( centerVert , outerRadius * curLJoystick.GetInnerDeadZoneFraction() );

	Vec3 rawCenter
	(
		center.x + curLJoystick.GetRawPosition().x * outerRadius
		,	center.y + curLJoystick.GetRawPosition().y * outerRadius
		,	0.0f	
	);
	Vertex_PCU rawInput( rawCenter, Rgba( 1.0f, 0.0f, 0.0f, 1.0f ), Vec2( 0.0f, 0.0f ) );
	DrawDisc( rawInput , posRadius );

	Vec3 fixedCenter
	(
		center.x + curLJoystick.GetPosition().x * outerRadius
		,	center.y + curLJoystick.GetPosition().y * outerRadius
		,	0.0f	
	);
	Vertex_PCU fixedInput( fixedCenter, Rgba( 0.0f, 0.7f, 0.7f, 1.0f ), Vec2( 0.0f, 0.0f ) );
	DrawDisc( fixedInput , posRadius );
}

//--------------------------------------------------------------------------
/**
* Render
*/
void App::Render() const
{
	g_theRenderer->ClearScreen( Rgba::BLACK );

	g_theGame->GameRender();
	
	ProfilerPush("imGUI");
	ProfilerDisplayRender();
	g_theImGUISystem->Render();
	ProfilerPop();

	if( g_theConsole->IsOpen() )
	{
		g_theConsole->Render( g_theRenderer, g_theGame->m_DevColsoleCamera, m_consoleTextHeight );
	}
	else
	{
		g_theDebugRenderSystem->RenderToScreen();
	}
}

//--------------------------------------------------------------------------
/**
* EndFrame
*/
void App::EndFrame()
{
	g_theConsole->		EndFrame();
	g_theAudioSystem->	EndFrame();
	g_theInputSystem->	EndFrame();
	g_theRenderer->		EndFrame();
	g_theEventSystem->	EndFrame();
	g_theImGUISystem->	EndFrame();
}

//--------------------------------------------------------------------------
/**
* ToggleDebug
*/
void App::ToggleDebug()
{
	if ( g_isInDebug )
	{	
		g_isInDebug = false;
	}
	else
	{
		g_isInDebug = true;
	}
}

//--------------------------------------------------------------------------
/**
* RegisterEvents
*/
void App::RegisterEvents()
{
	g_theEventSystem->SubscribeEventCallbackFunction( "quit", QuitEvent );
}

//--------------------------------------------------------------------------
/**
* GetGameClock
*/
Clock* App::GetGameClock() const
{
	return m_gameClock;
}

