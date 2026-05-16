#pragma once
#include <vector>

struct Vertex // Struct the holds the x y and z coordinates for each struct 
{
    float x;
    float y;
    float z;
};


class Mesh 
// A mesh is contructed by default with no vertecies they must be added via Add_Point
{

    public:

    Mesh() = default; 

    ~Mesh() = default;

    int CountVertices() // Method to return the amount of vertex points

    bool add_point(Vertex point); // add a point to a mesh (stored in the vertex vector)

    std::vector<Vertex> getallvertices();  

    Vertex getvertex(int index);

    //TODO perhaps a method to reset a mesh?   resetmesh()?


    private:

    std::vector<Vertex> vertices; 
    

};
