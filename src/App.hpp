
#pragma once

#include <iostream>
#include <cstdint>

#include "Window.hpp"

class App {

public:
    App();
    ~App() = default;

    int32_t run();

private:

    Window* window_;

    bool rendering_ = true;

};
