#include "Mesh.hpp"
#include <vector>
#include <iostream>
#include <string>


Mesh::Mesh(Logger& logs) : loggerref(logs) // assinging loggeref as the argument representing the logger object in the constructor
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
    int counter = 0;
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
 

void Mesh::offset(float amount) // Helper function that applies a offset to all of the vertices in a mesh.
{
for (Vertex& vertex : vertices) // refrencing the vertices directly
    {
    vertex.x = vertex.x + amount;
    vertex.y = vertex.y + amount;
    vertex.z = vertex.z + amount;
    }
return;
}

bool Mesh::CreateTriangle(float offsetamount, Vertex vertex1, Vertex vertex2, Vertex vertex3)
{

    std::string message = " ";
    add_point(vertex1);
    add_point(vertex2);
    add_point(vertex3);
    offset(offsetamount);


    message = "Triangle mesh created with a offset of ";
    message = message + std::to_string(offsetamount) = "\n";
    message = message + getxyz();
    loggerref.log("mesh", Logger::Flag::info, message);
    return true;
}

bool Mesh::CreateSquare(float offsetamount, Vertex vertex1, Vertex vertex2,  Vertex vertex3, Vertex vertex4)
{
        std::string message = " ";
        add_point(vertex1);
        add_point(vertex2);
        add_point(vertex3);
        add_point(vertex4);
        offset(offsetamount);

        message = "Square mesh created with a offset of ";
        message = message + std::to_string(offsetamount);
        message = message + " with vertices ";
        message = message + getxyz();
        loggerref.log("mesh", Logger::Flag::info, message);
        return true;
}