#pragma once
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"

class Entity;

enum TileType
{
	UNKNOWN_TILE_TYPE = -1,
	GRASS_TILE,
	SAND_TILE,
	GLOWSTONEPATH_TILE,
	STONE_TILE,
	LINESTONE_TILE,
	GEMSTONEWALL_TILE,
	MUD_TILE,
	QUICKSAND_TILE,
	SLIPGLOW_TILE,
	WATER_TILE,
	LAVA_TILE,
	COMMON_STONE_TILE,
	EXIT_TILE,
	NUM_TILE_TYPES
};



class TileDefinition
{
public:
	static TileDefinition* s_tileDefinitions[NUM_TILE_TYPES];

public:
	explicit TileDefinition( bool isSolid, bool isOpaque, const Rgba& tint, const Vec2& uvAtMin, const Vec2& uvAtMax, int spriteSheetIndex, float speedModifier = 1.0f );
	
	// Queries 
	bool		IsSolid()			const	{ return m_isSolid;			}
	bool		IsOpaque()			const	{ return m_opaque;			}
	float		GetSpeedModifier()	const	{ return m_speedModifier;	}
	int			GetTileIndex()		const	{ return m_tileIndex;		}
	const Rgba	GetTint()			const	{ return m_tint;			}
	const Vec2	GetuvAtMins()		const	{ return m_uvAtMin;			}
	const Vec2	GetuvAtMax()		const	{ return m_uvAtMax;			}

	static void InitiolizeDefinitions();
	static void DeconstructDefinitions();

private:
	static void CheckIfAllTilesInitialized();

private:
	bool	m_isSolid;
	bool	m_opaque;
	float	m_speedModifier;
	Rgba	m_tint;
	Vec2	m_uvAtMin;
	Vec2	m_uvAtMax;
	int		m_tileIndex;
};