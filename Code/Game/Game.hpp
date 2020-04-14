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
	bool IsInGodMode() { return m_godMode; };

public:
	// Helper functions to create reconstructed entities
	Entity* CreateExit( const Vec2& portal_from_loc, const Vec2& portal_to_loc, int from_map, int to_map, bool active, const std::string& texture, const Vec2& bot_left_uvs, const Vec2& top_right_uvs );
	Entity* CreateEnemy( const std::string& name, const Vec2& position, int map_idx, float damage_multiplier, float max_speed, float vision_radius, const std::string& texture, const Vec2& bot_left_uvs, const Vec2& top_right_uvs  );

public:
	Map* GetCurrentMap() const;
	Map* GetMap( int id ) const;

	void UpdateCamera( float deltaSeconds );

private:
	void HandleMapTransfers();

private:
	bool m_isQuitting = false;
	bool m_inDebug = false;
	bool m_godMode = false;

	Shader* m_shader;

	int m_map_id = -1;

	mutable Camera m_CurentCamera;
	mutable Camera m_DevColsoleCamera;

	SpriteSheet* m_terrain_sheet;

	std::unordered_map<int, Map*> m_maps;

};