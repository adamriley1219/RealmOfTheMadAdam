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
	: Component(), m_active( active ), m_minOrtho( min_ortho ), m_maxOrtho( max_ortho )
{
	m_camera = new Camera();
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
CameraComp::~CameraComp()
{
	delete m_camera;
}

//--------------------------------------------------------------------------
/**
* Copy
*/
void CameraComp::Copy(const Component* copyFrom)
{
	CameraComp* cam_copy = (CameraComp*)copyFrom;
	delete m_camera;
	m_camera = new Camera();
	m_minOrtho = cam_copy->m_minOrtho;
	m_maxOrtho = cam_copy->m_maxOrtho;
	m_camera->SetOrthographicProjection( m_minOrtho, m_maxOrtho );
	m_active = cam_copy->m_active;
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
	delete m_camera;
	m_camera = new Camera();
	m_camera->SetOrthographicProjection( m_minOrtho, m_maxOrtho );
	m_active = false;
}

//--------------------------------------------------------------------------
/**
* SetOrthProjMat
*/
void CameraComp::SetOrthProjMat( Vec2 min_ortho, Vec2 max_ortho )
{
	m_minOrtho = min_ortho;
	m_maxOrtho = max_ortho;
	m_camera->SetOrthographicProjection( min_ortho, max_ortho );
}
