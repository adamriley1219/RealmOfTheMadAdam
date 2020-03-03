#include "Game/Components/AIComp.hpp"


AIComp AIComp::s_protoAIComp;

//--------------------------------------------------------------------------
/**
* UIComp
*/
AIComp::AIComp()
	: Component()
{
	
}

//--------------------------------------------------------------------------
/**
* ~UIComp
*/
AIComp::~AIComp()
{

}

//--------------------------------------------------------------------------
/**
* Copy
*/
void AIComp::Copy(const Component* copyFrom)
{
	*this = *( (AIComp*)copyFrom );
}

//--------------------------------------------------------------------------
/**
* GetType
*/
eComponentType AIComp::GetType()
{
	return AI_COMP;
}

//--------------------------------------------------------------------------
/**
* Reset
*/
void AIComp::Reset()
{
	*this = s_protoAIComp;
}
