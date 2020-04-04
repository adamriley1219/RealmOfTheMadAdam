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
	virtual void Reset();

	void SetOrthProjMat( Vec2 min_ortho, Vec2 max_ortho );

public:
	Camera* m_camera	= nullptr;
	Vec2 m_minOrtho		= Vec2::ZERO;
	Vec2 m_maxOrtho		= Vec2::ONE;
	bool m_active		= false;

private:
	static CameraComp s_protoCameraComp;
};
