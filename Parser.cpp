#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Parser.h"
using namespace std;

//Read points from file (filePath) and save it into the vertice list (vertices).
bool parsePoints(const char* filePath, std::vector<Vertex> &vertices){
	float a, b, c;
	char v;
	string line;
	ifstream myfile(filePath);

	//Open file. Return true if there's no problem, else return false.
	if (myfile.is_open())
	{
		//Read line and save it in a string variable.
		while (getline(myfile, line))
		{
			//Process that line
			stringstream linestream(line);
			if (linestream >> a >> b >> c)
			{
				//Get the tree points of the line
				Vertex vertexLine;
				vertexLine.x = a;
				vertexLine.y = b;
				vertexLine.z = c;

				//Save this points into the vertices list
				vertices.push_back(vertexLine);

			}
		}
		//Close file and return.
		myfile.close();
		return true;
	}
	return false;
}
