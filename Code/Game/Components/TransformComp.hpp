#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Physics/Transform2D.hpp"

struct TransformComp : public Component
{
public:
	TransformComp();
	TransformComp( float x, float y );
	~TransformComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	Transform2D m_transform;

private:
	static TransformComp s_prototype;
};
