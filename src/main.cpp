
#include "App.hpp"
#include "Logger.hpp"
#include "Mesh.hpp"
#include "Pps.hpp"

int main(int argc, char** argv) 
{

    Logger Evan1(Logger::Flag::debug,true,true,true,true );
    Mesh Evan2(Evan1);
    Evan2.CreateTriangle(2,2,2);
    PPS Evan3(Evan1);
    Evan3.getsummary();


    // create app and run

    App app;
    app.run();
    
    // no freaking way

    return 0;
}
