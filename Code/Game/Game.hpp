#pragma once
#include "Game/GameCommon.hpp"

#include "Engine/Renderer/Camera.hpp"

#include <unordered_map>

class Shader;
class Map;
class SpriteSheet;

class Game
{
	friend App;
public:
	Game();
	~Game();

	void Startup();
	void Shutdown();

	bool HandleKeyPressed( unsigned char keyCode );
	bool HandleKeyReleased( unsigned char keyCode );

	void GameRender() const;
	void UpdateGame( float deltaSeconds );

	bool IsInDebug() { return m_inDebug; };

public:
	Map* GetCurrentMap() const;
	Map* GetMap( int id ) const;

	void UpdateCamera( float deltaSeconds );

private:
	void HandleMapTransfers();

private:
	bool m_isQuitting = false;
	bool m_inDebug = false;

	Shader* m_shader;

	int m_map_id = -1;

	mutable Camera m_CurentCamera;
	mutable Camera m_DevColsoleCamera;

	SpriteSheet* m_terrain_sheet;

	std::unordered_map<int, Map*> m_maps;

};