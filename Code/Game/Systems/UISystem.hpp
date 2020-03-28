#pragma once
#include "Game/Systems/System.hpp"

class Entity;

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

};


