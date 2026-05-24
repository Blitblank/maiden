#include "Mesh.hpp"
#include <vector>
#include <iostream>
#include <string>


Mesh::Mesh(Logger& logs) : loggerref(logs) // assinging loggeref as the argument representing the logger object in the constructor
{

}

bool Mesh::add_point(int x, int y, int z)
{

    Vertex point;
    point.x = x;
    point.y = y;
    point.z = z;

vertices.push_back(point);
loggerref.log("mesh", Logger::Flag::debug, "Was able to add a vertex.");
return true;
}


std::vector<Vertex> Mesh::getallvertices()
{
    loggerref.log("mesh", Logger::Flag::debug, "Was able to retrun all vertices.");
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
        loggerref.log("mesh", Logger::Flag::debug, "Was able to find the vertex.");
            return vertices[index]; // returns index 0 by default

    }

    else
    {
        loggerref.log("mesh", Logger::Flag::warning, "Unable to fetch the correct vertex point.");
        return vertices[0];// returns index 0 by default
    }  
    }



/*
Vertex Mesh::offset(Vertex vertex, float amount) // Helper function that applies a offset to each of the points in a vertex
{
    vertex.x = vertex.x + amount;
    vertex.y = vertex.y + amount;
    vertex.z = vertex.z + amount;
    return vertex;
}
 */


std::string Mesh::getxyz()  // Helper function that returns vertex points in string format for printing 
{
    std::string message = "";
    int counter = 1;
    for (Vertex vertex : vertices)
    {
    message = message + "Vertex " + std::to_string(counter) + " - ";
    message = message + "[";
    message = message + std::to_string(vertex.x);
    message = message + " ";
    message = message + std::to_string(vertex.y);
    message = message + " ";
    message = message + std::to_string(vertex.z);
    message = message + "]" + "\n";
    counter++;
    }

    message = message + " ";
    return message;
}
 

void Mesh::offset(float x, float y , float z) // Helper function that applies a offset to all of the vertices in a mesh.
{
for (Vertex& vertex : vertices) // refrencing the vertices directly
    {
    vertex.x = vertex.x + x;
    vertex.y = vertex.y + y;
    vertex.z = vertex.z + z;
    }
return;
}

void Mesh::changepoint(int index, int x, int y, int z)
{
    int i = 0;
    for (Vertex& vertex : vertices)
    {
       
        if(i == index)
        {
            vertex.x = x;
            vertex.y = y;
            vertex.z = z;
        }
        i++;

    }
}

bool Mesh::CreateTriangle(float xoffset, float yoffset, float zoffset)
{

    std::string message = "";


    add_point(0,0,1);
    add_point(0,1,0);
    add_point(1,0,0);


    offset(xoffset, yoffset, zoffset);


    message = "Triangle mesh created with a offset of ";
    message = message + "(";
        message = message + std::to_string(xoffset) + ",";
        message = message + std::to_string(yoffset) + ",";
        message = message + std::to_string(zoffset);
        message = message + ")";
    message = message  + "\n";
    message = message + getxyz();
    loggerref.log("mesh", Logger::Flag::info, message);
    return true;
}

bool Mesh::CreateSquare(float xoffset, float yoffset, float zoffset)
{
        std::string message = " ";
        add_point(1,0,0);
        add_point(0,1,0);
        add_point(0,0,0);
        add_point(1,1,0);
        offset(xoffset, yoffset, zoffset);

        message = "Square mesh created with a offset of ";
        message = message + "(";
        message = message + std::to_string(xoffset) + ",";
        message = message + std::to_string(yoffset) + ",";
        message = message + std::to_string(zoffset);
        message = message + ")";
        message = message + " with vertices ";
        message = message + getxyz();
        loggerref.log("mesh", Logger::Flag::info, message);
        return true;
}