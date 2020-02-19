#pragma once
#include "Game/Components/Component.hpp"

#include "Engine/Renderer/Camera.hpp"


struct CameraComp : public Component
{
public:
	CameraComp();
	CameraComp( Vec2 min_ortho, Vec2 max_ortho, bool active );
	~CameraComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();

public:
	Camera m_camera;
	bool m_active = false;
};
