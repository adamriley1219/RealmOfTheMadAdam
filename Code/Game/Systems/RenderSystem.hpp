#pragma once
#include "Game/Systems/System.hpp"

struct RenderComp;
struct TransformComp;
struct CameraComp;

struct RenderSystemTuple
{
	RenderComp* render_comp;
	TransformComp* transform_comp;
};

struct RenderUpdateTuple
{
	CameraComp* camera;
};

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Render() const;
	void Update( float deltaTime ) const;
private:

};
