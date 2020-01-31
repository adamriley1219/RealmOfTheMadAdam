#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"

#include <vector>

class RenderComp : public Component
{
public:
	RenderComp();
	~RenderComp();

private:
	std::vector<Vertex_PCU> m_verts;
};
