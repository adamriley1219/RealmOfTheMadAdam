#pragma once
#include "Game/Components/Component.hpp"

#include <Engine/Core/EngineCommon.hpp>

struct NameComp : public Component
{
public:
	NameComp();
	NameComp( const std::string& given_name );
	~NameComp();

public:
	std::string m_name = "Unused";
};
