#pragma once
#include "Game/Systems/System.hpp"

class RenderComp;
class TransformComp;

struct RenderSystemTuple
{
	RenderComp* rendercomp;
	TransformComp* transformcomp;
};

class RenderSystem : public System
{
public:
	RenderSystem();
	~RenderSystem();

	void Render() const;

private:

};
