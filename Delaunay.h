#pragma once

#include <vector>
#include <list>

#include "Mesh.h"

// Edge structure. Contains references to the two vertices of the edge
struct Edge
{
	int v1;
	int v2;
};

struct edgeBuffer
{
	std::vector< Edge > edges;
};

const double EPSILON = 0.000001;

//Return true if the point is inside the CircumCircle fixed by the triangle. Else, return false.
bool isInsideCircumCircle(const Vertex A, const Vertex B, const Vertex C, const Vertex &point);

//Get all the points from the list and triangulate by Delaunay and then fill the mesh by reference.
void triangulateMesh(Mesh &mesh);

void superTriangle(Mesh &mesh);

void addVertex(Mesh &mesh, const int vertexIndex);

void keepUniqueEdges(edgeBuffer &buffer);