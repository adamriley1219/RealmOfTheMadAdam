#pragma once
#include "Engine/Core/EngineCommon.hpp"


struct Vertex_PCU;
class Entity;
class EntityAdmin;
class Camera;
class Map;

typedef int EntityID;
//typedef int ComponentID;

void DrawDisc( const Vertex_PCU translation, float radius );
float GetRandomlyChosenFloat( float a, float b );
Map* GetCurrentMap();
EntityAdmin& GetCurrentAdmin();


