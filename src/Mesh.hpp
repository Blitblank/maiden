#pragma once
#include <vector>
#include "Logger.hpp"

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

    Mesh(Logger& logger); 

    ~Mesh() = default;

    int CountVertices(); // Method to return the amount of vertex points

    bool CreateTriangle(float offsetamount, Vertex vertex1, Vertex vertex2, Vertex vertex3); // automatically creates a mesh with three points 

    bool add_point(Vertex point); // add a point to a mesh (stored in the vertex vector)

    std::vector<Vertex> getallvertices();  

    Vertex getvertex(int index);

    bool CreateSquare(float offsetamount, Vertex vertex1, Vertex vertex2,  Vertex vertex3, Vertex vertex4);
   


    private:

    std::vector<Vertex> vertices; 
    
    Logger& loggerref;

    std::string getxyz();

    void offset(float amount);
};
