#include "Mesh.hpp"
#include <vector>
#include <iostream>



Mesh::Mesh(Logger& logger) : loggerref(logger) // assinging loggeref as the argument representing the logger object in the constructor
{

}

bool Mesh::add_point(Vertex point)
{
vertices.push_back(point);
loggerref.log("mesh", Logger::Flag::info, "Was able to add a vertex.");
return true;
}


std::vector<Vertex> Mesh::getallvertices()
{
    loggerref.log("mesh", Logger::Flag::info, "Was able to retrun all vertices.");
    return vertices;
}

int Mesh::CountVertices()
    {
        return vertices.size(); 
    }

Vertex Mesh::getvertex(int index)
    {

    if (!(index < 0 || index >= vertices.size())) //check to see if index is out of bounds

    {
        loggerref.log("mesh", Logger::Flag::info, "Was able to find the vertex.");
            return vertices[index]; // returns index 0 by default

    }

    else

    {
        loggerref.log("mesh", Logger::Flag::warning, "Unable to fetch the correct vertex point.");
        return vertices[0];// returns index 0 by default
    }  

    }


