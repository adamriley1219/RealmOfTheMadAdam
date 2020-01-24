#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Game/Game.hpp"
#include "Game/Tile.hpp"
#include "Game/Entity.hpp"
#include "Game/TileDefinition.hpp"

//--------------------------------------------------------------------------
/**
* Tile
*/
Tile::Tile( int tileID, TileType tileType, const IntVec2& position, const SpriteSheet* spriteSheetForTile )
	: m_tileID( tileID )
	, m_tileType( tileType )
	, m_position( position )
	, m_boarder( AABB2( (float) position.x, (float) position.y, (float) position.x + 1.0f, (float) position.y + 1.0f ) )
	, m_textureSpriteSheet( spriteSheetForTile )
{

}

//--------------------------------------------------------------------------
/**
* TileIsSolid
*/
bool Tile::IsSolid() const
{
	return TileDefinition::s_tileDefinitions[m_tileType]->IsSolid();
}


//--------------------------------------------------------------------------
/**
* GetSpeedModifier
*/
float Tile::GetSpeedModifier() const
{
	return TileDefinition::s_tileDefinitions[m_tileType]->GetSpeedModifier();
}

//--------------------------------------------------------------------------
/**
* PushOutOfTileIsSpawnable
*/
bool Tile::IsSpawnable() const
{
	return m_tileType == GRASS_TILE || m_tileType == SAND_TILE || m_tileType == GLOWSTONEPATH_TILE;
}

//--------------------------------------------------------------------------
/**
* AddVertsForRender
*/
void Tile::AddVertsForRender( std::vector<Vertex_PCU>& addTo )
{
	Vec2 bottomLeftUV	= Vec2::ZERO;
	Vec2 topRightUV		= Vec2::ONE;

	int tileSpriteSheetLocation = TileDefinition::s_tileDefinitions[m_tileType]->GetTileIndex();
	SpriteDefinition spriteDef = m_textureSpriteSheet->GetSpriteDefinition(tileSpriteSheetLocation);

	spriteDef.GetUVs( bottomLeftUV, topRightUV );

	Rgba color = TileDefinition::s_tileDefinitions[m_tileType]->GetTint();

	AddVertsForAABB2D( addTo, m_boarder, color, bottomLeftUV, topRightUV );
}

//--------------------------------------------------------------------------
/**
* SetSolid
*/
void Tile::SetType( TileType type)
{
	m_tileType = type;
}
//--------------------------------------------------------------------------
/**
* IsPointOnTile
*/
bool Tile::IsPointOnTile( const Vec2& point ) const
{
	if  ( 	point.x >= m_position.x
		&&	point.x <= m_position.x + 1.0f
		&&	point.y >= m_position.y
		&&	point.y <= m_position.y + 1.0f
		)
	{
		return true;
	}
	return false;
}
