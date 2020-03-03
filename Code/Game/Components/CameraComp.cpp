#include "Game/Components/CameraComp.hpp"


CameraComp CameraComp::s_protoCameraComp;

//--------------------------------------------------------------------------
/**
* UIComp
*/
CameraComp::CameraComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* CameraComp
*/
CameraComp::CameraComp( Vec2 min_ortho, Vec2 max_ortho, bool active )
	: Component(), m_active( active )
{
	m_camera.SetOrthographicProjection( min_ortho, max_ortho );
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
CameraComp::~CameraComp()
{
	
}

//--------------------------------------------------------------------------
/**
* Copy
*/
void CameraComp::Copy(const Component* copyFrom)
{
	*this = *( (CameraComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType CameraComp::GetType()
{
	return CAMERA_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void CameraComp::Reset()
{
	m_camera = Camera();
	m_active = false;
}
