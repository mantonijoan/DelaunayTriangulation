#include <iostream>
#include "Parser.h"
#include "Delaunay.h"

void main(int argc, const char* argv[])
{
	//Create the mesh object
	Mesh mesh;

	std::cout << "Parsing input file. Please wait...\n";

	//Read the text file and fill the list (points).
	if (!parsePoints("input.txt", mesh.vertices))
	{
		printf("ERROR! Could not parse input file\n");
		return;
	}

	//Send progress feedback
	std::cout << " [Done]\n" << std::endl;
	std::cout << "Creating mesh. Please wait...\n";

	//Call Delaunay Triangulation Algorithm
	triangulateMesh(mesh);

	//Send progress feedback
	std::cout << " [Done]\n" << std::endl;
	std::cout << "Saving PLY file. Please wait...\n";

	//Save mesh to PLY format
	if (!saveMeshToPLY(mesh, "ouput.ply"))
	{
		printf("ERROR! Could not save mesh to PLY file\n");
		return;
	}

	std::cout << " [Done]\n" << std::endl;
}

