#pragma once
#include "Game/Systems/System.hpp"

struct RenderComp;
struct TransformComp;

struct RenderSystemTuple
{
	RenderComp* render_comp;
	TransformComp* transform_comp;
};

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Render() const;

private:

};
