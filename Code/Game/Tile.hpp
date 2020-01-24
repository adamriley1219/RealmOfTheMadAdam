#pragma once
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Game/GameCommon.hpp"
#include "Game/TileDefinition.hpp"
#include <vector>

class Entity;
class SpriteSheet;

class Tile
{
public:
	explicit Tile( int tileID, TileType tileType, const IntVec2& position, const SpriteSheet* spriteSheetForTile );
	
	// Queries 
	bool	IsSolid()				const;
	float	GetSpeedModifier()		const;
	bool	IsSpawnable()			const;
	IntVec2 GetTileMapCoordinates()	const	{ return m_position; }
	int		GetTileIndex()			const	{ return m_tileID; }
	
	// Actions
	void AddVertsForRender( std::vector<Vertex_PCU>& addTo );
	void SetType( TileType type );

	bool IsPointOnTile( const Vec2& point ) const;

private:
	const SpriteSheet*	m_textureSpriteSheet;
	TileType			m_tileType;
	IntVec2				m_position;
	int					m_tileID;
	AABB2				m_boarder;
};