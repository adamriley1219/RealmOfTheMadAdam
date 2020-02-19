#pragma once
#include "Game/Systems/System.hpp"

struct RenderComp;
struct TransformComp;
struct CameraComp;

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Render() const;
	void Update( float deltaTime ) const;
private:

};
