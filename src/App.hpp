
#pragma once

#include <iostream>
#include <cstdint>

#include "Window.hpp"
#include "Engine.hpp"

class App {

public:
    App();
    ~App() = default;

    int32_t run();

private:

    Window* window_;
    Engine* engine_;

    bool rendering_ = true;

};
