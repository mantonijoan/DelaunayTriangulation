#include <iostream>
#include <list>
#include "Delaunay.h"


void triangulateMesh(Mesh &mesh){

	//Create supertriangle and add it to the triangulation
	superTriangle(mesh);

	//For each vertex (except the supertriangle's vertices)
	for (int i = 0; i < mesh.vertices.size() - 3; i++){


		//Add this vertex to triangulation
		addVertex(mesh, i);
		//Progress feedback
		if (i % 100 == 0) std::cout << i << " of " << mesh.vertices.size() << " completed.\n";
	}

	//Remove any triangles from the triangle list that use the supertriangle vertices
	for (int i = 0; i < mesh.faces.size(); i++){
		if ((mesh.faces[i].a == mesh.vertices.size() - 3) || (mesh.faces[i].a == mesh.vertices.size() - 2) || (mesh.faces[i].a == mesh.vertices.size() - 1) ||
			(mesh.faces[i].b == mesh.vertices.size() - 3) || (mesh.faces[i].b == mesh.vertices.size() - 2) || (mesh.faces[i].b == mesh.vertices.size() - 1) ||
			(mesh.faces[i].c == mesh.vertices.size() - 3) || (mesh.faces[i].c == mesh.vertices.size() - 2) || (mesh.faces[i].c == mesh.vertices.size() - 1))
		{
			//Remove that triangle
			mesh.faces.erase(mesh.faces.begin() + i);
			i = i - 1;
		}
	}
}

void superTriangle(Mesh &mesh){
	//Initialize bounding limits of our geometry
	double
		xmin = mesh.vertices[0].x,
		xmax = xmin,
		ymin = mesh.vertices[0].y,
		ymax = ymin;

	//Find the maximum and minimum vertex bounds. This allow calculation of the bounding triangle
	for (int i = 1; i < mesh.vertices.size(); i++){
		if (mesh.vertices[i].x < xmin) xmin = mesh.vertices[i].x;
		if (mesh.vertices[i].x > xmax) xmax = mesh.vertices[i].x;
		if (mesh.vertices[i].y < ymin) ymin = mesh.vertices[i].y;
		if (mesh.vertices[i].y > ymax) ymax = mesh.vertices[i].y;
	}

	//Find mid coordinates and set maximum distances.
	double
		dx = xmax - xmin,
		dy = ymax - ymin,
		dmax = (dx > dy) ? dx : dy,
		xmid = (xmax + xmin) / 2.0,
		ymid = (ymax + ymin) / 2.0;

	//Determine the supertriangle. This is a triangle which encompasses all the sample points.
	Vertex
		v1{ xmid - 20 * dmax, ymid - dmax, 0.0 },
		v2{ xmid + 20 * dmax, ymid - dmax, 0.0 },
		v3{ xmid, ymid + 20 * dmax, 0.0 };

	//Add supertriangle vertices to the end of the vertex list
	mesh.vertices.push_back(v1);
	mesh.vertices.push_back(v2);
	mesh.vertices.push_back(v3);

	//Add the supertriangle to the faces list. At this point mesh.faces is empty.
	mesh.faces.push_back(Face{ mesh.vertices.size() - 3, mesh.vertices.size() - 2, mesh.vertices.size() - 1 });

}

void addVertex(Mesh &mesh, const int vertexIndex){

	//Initialize the edge buffer
	edgeBuffer buffer;

	//For each triangle in faces list
	for (int i = 0; i < mesh.faces.size(); i++){

		Vertex 
			A = mesh.vertices[mesh.faces[i].a],
			B = mesh.vertices[mesh.faces[i].b],
			C = mesh.vertices[mesh.faces[i].c],
			P = mesh.vertices[vertexIndex];

		//If this vertex is inside triangle's circumscribed circle
		if (isInsideCircumCircle(A, B, C, P)){

			//Add the three triangle edges to the edge buffer
			buffer.edges.push_back(Edge{ mesh.faces[i].a, mesh.faces[i].b });
			buffer.edges.push_back(Edge{ mesh.faces[i].b, mesh.faces[i].c });
			buffer.edges.push_back(Edge{ mesh.faces[i].c, mesh.faces[i].a });

			//Remove that triangle
			mesh.faces.erase(mesh.faces.begin() + i);
			i--;
		}
	}

	//Remove double edges, keeping only the unique ones.
	keepUniqueEdges(buffer);

	//For all buffer edges
	for (int i = 0; i < buffer.edges.size(); i++){

		//Form new triangles between the remaining edges and the new vertex
		mesh.faces.push_back(Face{ buffer.edges[i].v1, buffer.edges[i].v2, vertexIndex });
	}
}

bool isInsideCircumCircle(const Vertex A, const Vertex B, const Vertex C, const Vertex &point){
	double m1, m2, mx1, mx2, my1, my2, xc, yc, r;
	double dx, dy, rsqr, drsqr;

	if (abs(A.y - B.y) < EPSILON && abs(B.y - C.y) < EPSILON)
		return(false);
	if (abs(B.y - A.y) < EPSILON){
		m2 = -(C.x - B.x) / (C.y - B.y);
		mx2 = (B.x + C.x) / 2.0;
		my2 = (B.y + C.y) / 2.0;
		xc = (B.x + A.x) / 2.0;
		yc = m2 * (xc - mx2) + my2;
	}
	else if (abs(C.y - B.y) < EPSILON){
		m1 = -(B.x - A.x) / (B.y - A.y);
		mx1 = (A.x + B.x) / 2.0;
		my1 = (A.y + B.y) / 2.0;
		xc = (C.x + B.x) / 2.0;
		yc = m1 * (xc - mx1) + my1;
	}
	else{
		m1 = -(B.x - A.x) / (B.y - A.y);
		m2 = -(C.x - B.x) / (C.y - B.y);
		mx1 = (A.x + B.x) / 2.0;
		mx2 = (B.x + C.x) / 2.0;
		my1 = (A.y + B.y) / 2.0;
		my2 = (B.y + C.y) / 2.0;
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}
	dx = B.x - xc;
	dy = B.y - yc;
	rsqr = dx * dx + dy * dy;
	dx = point.x - xc;
	dy = point.y - yc;
	drsqr = dx * dx + dy * dy;
	return((drsqr <= rsqr) ? true : false);

}

void keepUniqueEdges(edgeBuffer &buffer){

	//For all edges
	for (int i = 0; i < buffer.edges.size(); i++){

		//Initialize searching flag (not founded)
		bool found = false;

		for (int j = 0; j < buffer.edges.size() && !found; j++){

			//If the edge is not himself
			if (i != j){

				//Find the same edge
				if ((buffer.edges[i].v1 == buffer.edges[j].v1 && buffer.edges[i].v2 == buffer.edges[j].v2) ||
					(buffer.edges[i].v1 == buffer.edges[j].v2 && buffer.edges[i].v2 == buffer.edges[j].v1)){

					//Remove that two edges from buffer edge
					buffer.edges.erase(buffer.edges.begin() + j);
					buffer.edges.erase(buffer.edges.begin() + i);
					i--;

					//Stop searching (founded)
					found = true;
				}
			}
		}
	}	
}



