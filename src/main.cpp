
#include "App.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "Pps.hpp"

int main(int argc, char** argv) 
{


#include "ConfigService.hpp"

int main(int argc, char** argv) 
{

    ConfigService config;
    config.loadFromFile("config/maiden.cfg");
    Logger logger(&config, "Engine");

    Vertex y; y.x = 1.2; y.y = 4.2; y.z = 5.2;
    Vertex x; x.x = 12.2; x.z = 54.2; x.y = 43.2;
    Vertex z; z.x = 14.2; z.y = 45.2; z.z = 56.2;

    Mesh Evan2(logger);
    Mesh Evan3(logger);
    Evan2.CreateTriangle(3.4,x,y,z);
    Evan3.CreateTriangle(4235.53,x,y,z);
    // create app and run

    App app;
    app.run();
    
    // no freaking way

    return 0;
}
