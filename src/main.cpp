
#include "App.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "Pps.hpp"

int main(int argc, char** argv) 
{
 // Test cases 
    Vertex y;

    y.x = 1.2;

    y.y = 4.2;

    y.z = 5.2;

    Vertex x;

    x.x = 12.2;

    x.y = 43.2;

    x.z = 54.2;

    Vertex z;

    z.x = 14.2;

    z.y = 45.2;

    z.z = 56.2;

    Logger Evan(Logger::Flag::info, true, true, "YOUR FILE PATH HERE", true, false); // Minimum level is set to warning so no debug or info messages should appear

    Mesh Evan2(Evan);
    Mesh Evan3(Evan);
    PPS Evan4(Evan);

    Evan2.CreateTriangle(3.4,x,y,z);

    Evan3.CreateTriangle(4235.53,x,y,z);

    Evan4.getmemoryusage();

    Evan4.getfps();

    Evan4.getframetime();

    Evan4.getcpuload();

    Evan4.getgpuload();

    Evan4.getmemoryusage();

    Evan4.getthreadcount();


    // create app and run

    App app;
    app.run();
    
    // no freaking way

    return 0;
}
