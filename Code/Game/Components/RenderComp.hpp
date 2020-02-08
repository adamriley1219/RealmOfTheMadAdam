#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Engine/Physics/Transform2D.hpp"

#include <vector>
#include <map>

struct Vertex_Info_Group 
{
	std::vector<Vertex_PCU> verts;
	Transform2D transform;
	bool is_text = false;
};

struct RenderComp : public Component
{
public:
	RenderComp();
	~RenderComp();

public:
	std::map< std::string, Vertex_Info_Group > m_verts_groups;
};
