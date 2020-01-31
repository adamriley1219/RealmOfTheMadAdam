#pragma once

struct Vertex_PCU;
class Entity;
class Camera;
class Map;

void DrawDisc( const Vertex_PCU translation, float radius );
float GetRandomlyChosenFloat( float a, float b );
Map* GetCurrentMap();


