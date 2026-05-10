
#pragma once

#include "Window.hpp"

class Engine {

public:

    Engine(Window* window);
    ~Engine() = default;

    void init();
    
    // draw is called every render iteration in that while loop
    void draw();

private:

    // might get rid of this

    Window* window_;

};
