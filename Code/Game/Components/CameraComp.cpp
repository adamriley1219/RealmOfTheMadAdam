#include "Game/Components/CameraComp.hpp"


//--------------------------------------------------------------------------
/**
* UIComp
*/
CameraComp::CameraComp()
	: Component()
{
	m_type = CAMERA_COMP;
}

//--------------------------------------------------------------------------
/**
* CameraComp
*/
CameraComp::CameraComp( Vec2 min_ortho, Vec2 max_ortho, bool active )
	: Component(), m_active( active )
{
	m_type = CAMERA_COMP;
	m_camera.SetOrthographicProjection( min_ortho, max_ortho );
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
CameraComp::~CameraComp()
{
	
}
