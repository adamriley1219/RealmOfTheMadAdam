#pragma once
#include "Game/Components/Component.hpp"

#include <Engine/Core/EngineCommon.hpp>

struct NameComp : public Component
{
public:
	NameComp();
	NameComp( const std::string& given_name );
	~NameComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	std::string m_name = "Unused";
private:
	static NameComp s_prototype;
};
