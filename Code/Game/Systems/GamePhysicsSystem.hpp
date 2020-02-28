#pragma once
#include "Game/Systems/System.hpp"

#include "Game/Components/TransformComp.hpp"
#include "Game/Components/PhysicsComp.hpp"
#include "Game/Tile.hpp"

class GamePhysicsSystem : public System
{
public:
	GamePhysicsSystem();
	~GamePhysicsSystem();

	void Update( float deltaTime ) const;

private:
	Vec2 GetPushAmountWithDirDynamicVsDynamic( const PhysicsComp& phys_comp_a, const TransformComp& trans_comp_a, const PhysicsComp& phys_comp_b, const TransformComp& trans_comp_b ) const;
	Vec2 GetPushAmountWithDirDynamicVsStatic( const PhysicsComp& phys_comp_dynamic, const TransformComp& trans_comp_dynamic, const PhysicsComp& phys_comp_static, const TransformComp& trans_comp_static ) const;
	void TryToPushOutOfTile( const PhysicsComp& phys_comp, TransformComp& trans_comp, const Tile& tile ) const;

};
