#pragma once
#include "Game/Components/Component.hpp"

#include <Engine/Core/EngineCommon.hpp>

class NameComp
{
public:
	NameComp();
	NameComp( const std::string& given_name );
	~NameComp();

	const std::string& GetName() const { return m_name; };
	void SetName( const std::string& name ) { m_name = name; };

private:
	std::string m_name = "Unused";
};
