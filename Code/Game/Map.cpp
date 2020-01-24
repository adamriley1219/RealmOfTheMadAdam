#include "Engine/Math/RNG.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"
#include "Game/Map.hpp"
#include "Game/Tile.hpp"
#include "Game/TileDefinition.hpp"
#include <vector>


//--------------------------------------------------------------------------
/**
* Map
*/
Map::Map( int mapID, IntVec2 mapDimensions, SpriteSheet* tileSpriteSheet
	, TileType filledWith, TileType solidWith )
	: m_mapID( mapID )
	, m_tileSpriteSheet( tileSpriteSheet )
{
	m_filledWith = filledWith;
	m_solidWith = solidWith;

	m_dimensions = mapDimensions;
	ConstructMap();
}

//--------------------------------------------------------------------------
/**
* ~Map
*/
Map::~Map()
{
	ClearMap();
}

//--------------------------------------------------------------------------
/**
* MapUpdate
*/
void Map::Update( float deltaSeconds )
{
	
}

//--------------------------------------------------------------------------
/**
* Render
*/
void Map::Render() const
{
	g_theRenderer->BindTextureView( 0, m_tileSpriteSheet->GetTextureView() );
	std::vector<Vertex_PCU> verts;
	for( int i = 0; i < m_tiles.size() ; i++ )
	{
		m_tiles[i]->AddVertsForRender( verts );
	}
	g_theRenderer->DrawVertexArray( (int) verts.size(), &verts[0] );	
}

//--------------------------------------------------------------------------
/**
* LevelComplete
*/
bool Map::LevelComplete() const
{
	return m_mapComplete;
}

//--------------------------------------------------------------------------
/**
* MapResetMap
*/
void Map::ResetMap()
{
	ClearMap();
	ConstructMap();
}

//--------------------------------------------------------------------------
/**
* ConstructMapGetTilePointIsOn
*/
const Tile* Map::GetTilePointIsOn( const Vec2& point ) const
{
	int tileIndex = GetTileIndexFromTileMapCoordinates( IntVec2( (int) point.x, (int) point.y) ); // Note: Only works because positions are always positive in this game.
	
	if( tileIndex < (int) m_tiles.size() && tileIndex >= 0 )
	{
		return m_tiles[tileIndex];
	}
	return nullptr;
}

//--------------------------------------------------------------------------
/**
* GetTilePointIsOnGetTileByMapCoordinates
*/
const Tile* Map::GetTileByMapCoordinates( int x, int y ) const
{
	int index = GetTileIndexFromTileMapCoordinates( IntVec2( x, y ) );
	if( index >= (int) m_tiles.size() || index < 0 )
	{
		return nullptr;
	}
	return m_tiles[ index ];
}


//--------------------------------------------------------------------------
/**
* ClearMap
*/
void Map::ClearMap()
{
	for( int i = 0; i < m_tiles.size(); i++ )
	{
		if( m_tiles[i] )
		{
			delete m_tiles[i];
			m_tiles[i] = nullptr;
		}
	}
	m_tiles.clear();
}

//--------------------------------------------------------------------------
/**
* ConstructMap
*/
void Map::ConstructMap()
{
	GUARANTEE_RECOVERABLE( m_tiles.empty(), Stringf( "Trying to construct a Map on Map with ID:%d when map was already made.", m_mapID ) );
	
	// Fill with Grass
	for( int yIndex = 0; yIndex < m_dimensions.y ; yIndex++ )
	{
		for( int xIndex = 0; xIndex < m_dimensions.x; xIndex++ )
		{
			m_tiles.push_back( new Tile( (int) m_tiles.size(), m_filledWith, IntVec2( xIndex, yIndex ), m_tileSpriteSheet ) );
		}
	}

	// Boarder the Zone
	for( int yIndex = 0; yIndex < m_dimensions.y ; yIndex++ )
	{
		for( int xIndex = 0; xIndex < m_dimensions.x ; xIndex += m_dimensions.x - 1 )
		{
			m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( xIndex, yIndex ) )]->SetType( m_solidWith );
		}
	}
	for( int yIndex = 0; yIndex < m_dimensions.y ; yIndex += m_dimensions.y - 1  )
	{
		for( int xIndex = 0; xIndex < m_dimensions.x ; xIndex++ )
		{
			m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( xIndex, yIndex ) )]->SetType( m_solidWith );
		}
	}

	// Create Bunker
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( 4, 2 ) )]->SetType(m_solidWith);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( 4, 3 ) )]->SetType(m_solidWith);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( 4, 4 ) )]->SetType(m_solidWith);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( 3, 4 ) )]->SetType(m_solidWith);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( 2, 4 ) )]->SetType(m_solidWith);



	// Create end
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( m_dimensions.x - 2, m_dimensions.y - 2 ) )]->SetType(EXIT_TILE);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( m_dimensions.x - 3, m_dimensions.y - 2 ) )]->SetType(COMMON_STONE_TILE);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( m_dimensions.x - 3, m_dimensions.y - 3 ) )]->SetType(COMMON_STONE_TILE);
	m_tiles[GetTileIndexFromTileMapCoordinates( IntVec2( m_dimensions.x - 2, m_dimensions.y - 3 ) )]->SetType(COMMON_STONE_TILE);

}

//--------------------------------------------------------------------------
/**
* ConstructMapGetTileIndexFromTileMapCoordinates
*/
int Map::GetTileIndexFromTileMapCoordinates( const IntVec2& mapCoordinates ) const
{
	return mapCoordinates.x + mapCoordinates.y * m_dimensions.x;
}

//--------------------------------------------------------------------------
/**
* GetTileIndexFromTileMapCoordinatesGetTileMapCoordinatesFromTileIndex
*/
IntVec2 Map::GetTileMapCoordinatesFromTileIndex( int index ) const
{
	int tileY = index / m_dimensions.x;
	int tileX = index - ( tileY * m_dimensions.x );
	return IntVec2( tileX, tileY );
}

//--------------------------------------------------------------------------
/**
* GetTileMapCoordinatesFromTileIndexGetRandomIndex
*/
int Map::GetRandomIndex()
{
	return g_theRNG->GetRandomIntInRange( 0, (int) m_tiles.size() - 1 );
}
