#pragma once

class System
{
public:
	System();
	virtual ~System();

	// Note: No function on a system should change a system, only components
	virtual void Update( float deltaTime ) const;
	virtual void Render() const;

};

