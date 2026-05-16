#include "Mesh.hpp"
#include <vector>

bool Mesh::add_point(Vertex point)

{

vertices.push_back(point);

return true;

}


std::vector<Vertex> Mesh::getvertices()
{
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
            return vertices[index]; // returns index 0 by default
        }

    else if (!(vertices.empty()))

    {
        std::cout<<" Unable to get vertex at index "<< index  << std::endl; // replace with logger 
        return vertices[0];// returns index 0 by default
        
    } 
    }


