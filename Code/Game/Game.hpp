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

public:
	Map* GetCurrentMap() const;

	void UpdateCamera( float deltaSeconds );

private:
	bool m_isQuitting = false;

	Shader* m_shader;

	uint m_map_id = 0;

	mutable Camera m_CurentCamera;
	mutable Camera m_DevColsoleCamera;

	SpriteSheet* m_terrain_sheet;

	std::unordered_map<uint, Map*> m_maps;

};