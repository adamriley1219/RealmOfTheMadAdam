#pragma once
#include "Game/Components/Component.hpp"
#include "Engine/Math/Vec2.hpp"

struct PhysicsComp : public Component
{
public:
	PhysicsComp();
	PhysicsComp( bool static_object );
	~PhysicsComp();

	virtual void Copy(const Component* copyFrom);
	virtual eComponentType GetType();
	virtual void Reset();

public:
	float	m_friction_per_sec		= 10.0f;
	float	m_acceleration			= 50.0f;
	float	m_max_speed				= 2.0f;
	Vec2	m_velocity				= Vec2::ZERO;
	float	m_radius				= .25f;
	bool	m_static_object			= true;

	// Used by physics system to keep track of how much to move objects out.
	Vec2	m_push_out_dir_amount = Vec2::ZERO;

private:
	static PhysicsComp s_prototype;

};
