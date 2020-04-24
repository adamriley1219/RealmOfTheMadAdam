#include "Game/Systems/GamePhysicsSystem.hpp"
#include "Game/Components/TriggerComp.hpp"

#include "Game/Map.hpp"
#include "Game/Game.hpp"



//--------------------------------------------------------------------------
/**
* GamePhysicsSystem
*/
GamePhysicsSystem::GamePhysicsSystem()
	: System()
{

}

//--------------------------------------------------------------------------
/**
* ~GamePhysicsSystem
*/
GamePhysicsSystem::~GamePhysicsSystem()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void GamePhysicsSystem::Update( float deltaTime ) const
{
	PROFILE_FUNCTION();
	UNUSED(deltaTime);
	Map& map = *g_theGame->GetCurrentMap();

	// Push out of each other or enlist a trigger
	EntityAdmin& admin = GetCurrentAdmin();
	for( Entity& entity : admin.m_entities )
	{
		if ( !entity.m_claimed )
		{
			continue;
		}

		PhysicsComp* physics_comp = (PhysicsComp*) entity.GetComponent( PHYSICS_COMP );
		TriggerComp* trigger_comp = (TriggerComp*) entity.GetComponent( TRIGGER_COMP );
		bool is_trigger = trigger_comp;

		if( physics_comp && ( physics_comp->m_static_object || is_trigger ) )
		{
			// Don't move static objects or deal with triggers
			continue;
		}

		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		if( physics_comp && trans_comp )
		{

			physics_comp->m_push_out_dir_amount = Vec2::ZERO;

			// Check on other entities
			for (Entity& other_entity : admin.m_entities)
			{
				// don't check on self
				if( !other_entity.m_claimed || other_entity.m_id == entity.m_id )
				{
					continue;
				}

				PhysicsComp* other_physics_comp = (PhysicsComp*)other_entity.GetComponent( PHYSICS_COMP );
				TransformComp* other_trans_comp = (TransformComp*)other_entity.GetComponent( TRANSFORM_COMP );
				TriggerComp* other_trigger_comp = (TriggerComp*)other_entity.GetComponent( TRIGGER_COMP );
				// Check if I can push out from other entity
				if( other_physics_comp && other_trans_comp )
				{
					bool other_is_trigger = other_trigger_comp;

					Vec2 push_amount = Vec2::ZERO;
					if( other_physics_comp->m_static_object )
					{
						push_amount = GetPushAmountWithDirDynamicVsStatic( *physics_comp, *trans_comp, *other_physics_comp, *other_trans_comp );
					}
					else
					{
						push_amount = GetPushAmountWithDirDynamicVsDynamic( *physics_comp, *trans_comp, *other_physics_comp, *other_trans_comp );
					}

					if( push_amount != Vec2::ZERO )
					{
						if( !other_is_trigger )
						{
							physics_comp->m_push_out_dir_amount += push_amount;
						}
					}
				}
			}
		}
	}


	// Apply movements
	for( Entity& entity : admin.m_entities )
	{
		if( !entity.m_claimed )
		{
			continue;
		}

		PhysicsComp* physics_comp = (PhysicsComp*)entity.GetComponent(PHYSICS_COMP);
		if( physics_comp && physics_comp->m_static_object)
		{
			// Don't move static objects
			continue;
		}
		TransformComp* trans_comp = (TransformComp*) entity.GetComponent( TRANSFORM_COMP );
		AbilityComp* ability_comp = (AbilityComp*) entity.GetComponent( ABILITY_COMP );

		if( physics_comp && trans_comp )
		{
			// Check against map tiles. Done here to force object fully out of solid tiles.
			const Tile* tile = map.GetTilePointIsOn(trans_comp->m_transform.m_position);
			if( tile )
			{
				// We are within the map.
				HandleCollisionWithTile( *physics_comp, *trans_comp, *tile, ability_comp, &entity );
				
				IntVec2 cur_tile_coords = tile->GetTileMapCoordinates();

				// Check left
				if( cur_tile_coords.x > 0 )
				{
					const Tile* adjacent_tile = map.GetTileByMapCoordinates( cur_tile_coords.x - 1, cur_tile_coords.y );
					if ( adjacent_tile )
					{
						HandleCollisionWithTile( *physics_comp, *trans_comp, *adjacent_tile, ability_comp, &entity);
					}
				}

				// Check down
				if( cur_tile_coords.y > 0 )
				{
					const Tile* adjacent_tile = map.GetTileByMapCoordinates( cur_tile_coords.x, cur_tile_coords.y - 1 );
					if ( adjacent_tile )
					{
						HandleCollisionWithTile( *physics_comp, *trans_comp, *adjacent_tile, ability_comp, &entity);
					}
				}

				// Check right
				if( cur_tile_coords.x < map.GetWidth() )
				{
					const Tile* adjacent_tile = map.GetTileByMapCoordinates( cur_tile_coords.x + 1, cur_tile_coords.y );
					if ( adjacent_tile )
					{
						HandleCollisionWithTile( *physics_comp, *trans_comp, *adjacent_tile, ability_comp, &entity);
					}
				}

				// Check up
				if( cur_tile_coords.y < map.GetHeight() )
				{
					const Tile* adjacent_tile = map.GetTileByMapCoordinates( cur_tile_coords.x, cur_tile_coords.y + 1 );
					if( adjacent_tile )
					{
						HandleCollisionWithTile( *physics_comp, *trans_comp, *adjacent_tile, ability_comp, &entity );
					}
				}
			}
		}
	}

}

//--------------------------------------------------------------------------
/**
* GetPushAmountDynamicVsDynamic
*/
Vec2 GamePhysicsSystem::GetPushAmountWithDirDynamicVsDynamic( const PhysicsComp& phys_comp_a, const TransformComp& trans_comp_a, const PhysicsComp& phys_comp_b, const TransformComp& trans_comp_b ) const
{
	const Vec2& pos_a = trans_comp_a.m_transform.m_position;
	const Vec2& pos_b = trans_comp_b.m_transform.m_position;
	float radius_a = phys_comp_a.m_radius;
	float radius_b = phys_comp_b.m_radius;


	if( DoDiscsOverlap( pos_a, radius_a, pos_b, radius_b ) )
	{
		Vec2 disp_pos_b_to_pos_a = pos_a - pos_b;

		float combined_radii = radius_a + radius_b;
		float penetration_distance = combined_radii - disp_pos_b_to_pos_a.GetLength();
		if( disp_pos_b_to_pos_a == Vec2::ZERO )
		{
			return Vec2::ZERO;
		}
		disp_pos_b_to_pos_a.SetLength(penetration_distance);

		return disp_pos_b_to_pos_a;
	}

	return Vec2::ZERO;
}

//--------------------------------------------------------------------------
/**
* GetPushAmountDynamicVsStatic
*/
Vec2 GamePhysicsSystem::GetPushAmountWithDirDynamicVsStatic( const PhysicsComp& phys_comp_dynamic, const TransformComp& trans_comp_dynamic, const PhysicsComp& phys_comp_static, const TransformComp& trans_comp_static ) const
{
	const Vec2& dynamic_pos = trans_comp_dynamic.m_transform.m_position;
	const Vec2& static_pos = trans_comp_static.m_transform.m_position;
	float dynamic_radius = phys_comp_dynamic.m_radius;
	float static_radius = phys_comp_static.m_radius;


	if( DoDiscsOverlap( dynamic_pos, dynamic_radius, static_pos, static_radius ) )
	{
		Vec2 disp_static_to_dyn = dynamic_pos - static_pos;

		float combined_radii = static_radius + dynamic_radius;
		float penetration_distance = combined_radii - disp_static_to_dyn.GetLength();
		if( disp_static_to_dyn == Vec2::ZERO )
		{
			return disp_static_to_dyn;
		}
		disp_static_to_dyn.SetLength( penetration_distance );

		return disp_static_to_dyn;
	}

	return Vec2::ZERO;
}

//--------------------------------------------------------------------------
/**
* PushOutOfTile
*/
bool  GamePhysicsSystem::TryToPushOutOfTile( const PhysicsComp& phys_comp, TransformComp& trans_comp, const Tile& tile ) const
{
	if( !tile.IsSolid() )
	{
		return false;
	}

	const AABB2& boarder = tile.GetBoarder();
	Vec2& position = trans_comp.m_transform.m_position;

	Vec2 closest_point_on_tile = GetClosestPointOnAABB2( position, boarder );
	Vec2 displacement_from_tile_to_position = position - closest_point_on_tile;

	float disp_lenth_squared = displacement_from_tile_to_position.GetLengthSquared();
	float radius_squared = phys_comp.m_radius * phys_comp.m_radius;
	if( disp_lenth_squared < radius_squared && disp_lenth_squared > 0.0f )
	{
		float distance_to_push = phys_comp.m_radius - sqrt( disp_lenth_squared );

		displacement_from_tile_to_position.SetLength( distance_to_push );

		position += displacement_from_tile_to_position;
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------
/**
* HandleCollisionWithTile
*/
void GamePhysicsSystem::HandleCollisionWithTile( const PhysicsComp& phys_comp, TransformComp& trans_comp, const Tile& tile, AbilityComp* ability_comp /*= nullptr*/, Entity* entity /*= nullptr*/ ) const
{
	if ( TryToPushOutOfTile( phys_comp, trans_comp, tile ) )
	{
		if ( ability_comp && entity )
		{
			entity->m_destroy = true;
		}
	}
}
