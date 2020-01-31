#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"

#include <vector>

struct RenderComp : public Component
{
public:
	RenderComp();
	~RenderComp();

public:
	std::vector<Vertex_PCU> m_verts;
};
