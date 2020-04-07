#pragma once
#include "Game/Systems/System.hpp"

class Entity;

constexpr int FPS_ARRAY_AVG_SIZE = 30;

class UISystem : public System
{
public:
	UISystem();
	~UISystem();

	virtual void Render() const;

private:
	void RenderCarrierUI( Entity& carrier ) const;
	void RenderGiverUI( Entity& giver ) const;
	void RenderGiverWithCarrierUI( Entity& giver, Entity& carrier ) const;
	void RenderPortalWithCarrierUI( Entity& portal, Entity& carrier ) const;

	const float m_alignment_modifier = 0.7f;

	mutable int cur_fps_idx = 0;
	mutable int fps_array[FPS_ARRAY_AVG_SIZE] = { 60 };

};


