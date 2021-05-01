#include "flame.h"
const int plane_001NAttribsPerVertex = 8;
const int plane_001NVertices = 4;
const int plane_001NTriangles = 2;
const float plane_001Vertices[] = {
  -1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,
  1.0f,-1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
  1.0f,1.0f,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f,
  -1.0f,1.0f,0.0f,-0.0f,0.0f,1.0f,0.0f,1.0f,
}; // end plane_001Vertices

const unsigned plane_001Triangles[] = {
  0, 1, 2,
  0, 2, 3,
}; // end plane_001Triangles

