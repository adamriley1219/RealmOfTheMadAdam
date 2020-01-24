#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/AABB2.hpp"	   
#include "Game/TileDefinition.hpp"


TileDefinition* TileDefinition::s_tileDefinitions[NUM_TILE_TYPES] = {};


//--------------------------------------------------------------------------
/**
* Tile
*/
TileDefinition::TileDefinition( bool isStone, bool isOpaque, const Rgba& tint, const Vec2& uvAtMin, const Vec2& uvAtMax, int spriteSheetIndex, float speedModifier )
	: m_isSolid(isStone), m_opaque(isOpaque), m_tint(tint), m_uvAtMin(uvAtMin), m_uvAtMax(uvAtMax), m_tileIndex(spriteSheetIndex), m_speedModifier(speedModifier)
{

}

//--------------------------------------------------------------------------
/**
* InitiolizeDefinitions
*/
void TileDefinition::InitiolizeDefinitions()
{
	DeconstructDefinitions();

	s_tileDefinitions[GRASS_TILE]			= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 0 );
	s_tileDefinitions[SAND_TILE]			= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 15 );
	s_tileDefinitions[GLOWSTONEPATH_TILE]	= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 59 );

	s_tileDefinitions[STONE_TILE]			= new TileDefinition( true,		false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 30 );
	s_tileDefinitions[LINESTONE_TILE]		= new TileDefinition( true,		false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 19 );
	s_tileDefinitions[GEMSTONEWALL_TILE]	= new TileDefinition( true,		false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 54 );

	s_tileDefinitions[MUD_TILE]				= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 21, 0.5f );
	s_tileDefinitions[QUICKSAND_TILE]		= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 23, 0.3f );
	s_tileDefinitions[SLIPGLOW_TILE]		= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 60, 0.1f );

	s_tileDefinitions[WATER_TILE]			= new TileDefinition( true,		false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 61 );
	s_tileDefinitions[LAVA_TILE]			= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 62 );
	s_tileDefinitions[COMMON_STONE_TILE]	= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 51 );
	s_tileDefinitions[EXIT_TILE]			= new TileDefinition( false,	false,	Rgba( 1.0f, 1.0f, 1.0f ),	Vec2( 0.0f, 0.0f),	Vec2(1.0f, 1.0f), 57 );

	CheckIfAllTilesInitialized();
}

//--------------------------------------------------------------------------
/**
* DeconstructDefinitions
*/
void TileDefinition::DeconstructDefinitions()
{
	for( int tileIndex = 0; tileIndex < NUM_TILE_TYPES; tileIndex++ )
	{
		if( s_tileDefinitions[tileIndex] )
		{
			delete s_tileDefinitions[tileIndex];
			s_tileDefinitions[tileIndex] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------
/**
* InitiolizeDefinitionsCheckIfAllTilesInitialized
*/
void TileDefinition::CheckIfAllTilesInitialized()
{
	for( int tileIndex = 0; tileIndex < NUM_TILE_TYPES; tileIndex++ )
	{
		GUARANTEE_RECOVERABLE( s_tileDefinitions[tileIndex], Stringf("TileDefinitoin %d is not initialized", tileIndex) );
	}
}
