#pragma once
#include <vector>
#include "Engine/Math/IntVec2.hpp"
#include "Game/Entity.hpp"
#include "Game/TileDefinition.hpp"
#include "Game/GameCommon.hpp"
#include "Game/EntityAdmin.hpp"

class Tile;
class PlayerTank;
class SpriteSheet;
struct RaycastResult;

class Map
{
public:
	explicit Map( int mapID, IntVec2 mapDimensions, SpriteSheet* tileSpriteSheet
			, TileType filledWith , TileType solidWith );
	~Map();

	void Update( float deltaSeconds );
	void Render() const;

	// Tile Functions
	const Tile* GetTilePointIsOn( const Vec2& point ) const;
	const Tile* GetTileByMapCoordinates( int x, int y ) const;

public:
	EntityAdmin m_admin;

private:
	// Map Manipulation 
	void ClearMap();
	void ConstructMap();

private:
	// Helper
	int		GetTileIndexFromTileMapCoordinates( const IntVec2& mapCoordinates ) const;
	IntVec2 GetTileMapCoordinatesFromTileIndex( int index ) const;
	int		GetRandomIndex();

private:
	const int					m_mapID;
	IntVec2						m_dimensions;
	std::vector<Tile*>			m_tiles;
	SpriteSheet*				m_tileSpriteSheet;
	bool						m_mapComplete = false;

	// Save for construction
	TileType					m_filledWith;
	TileType					m_solidWith;
};