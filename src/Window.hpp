
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan_raii.hpp>
<<<<<<< HEAD
=======

#include "Logger.hpp"
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d

// reference: https://wiki.libsdl.org/SDL3/SDL_CreateWindow
class Window {

public:

    Window(Logger* logger);
    ~Window();

    // for SDL3 event polling, runs once per app iteration in its while(running) loop
    void handleEvent(SDL_Event& event);

    bool rendering() { return rendering_; }
    bool open() { return open_; }
    bool createSurface(vk::raii::Instance* instance, vk::raii::SurfaceKHR* surface);
    bool getExtent(int32_t* width, int32_t* height);

private:

    // launches window, runs at app startup
    int init();

    // this window class will eventually hold mouse, keyboard, audio, etc. interfaces, like an SDL3 hub
    // app will be able to attach callbacks for mouse and keyboard events

    Logger* logger_;

    SDL_Window* sdlWindow_;

    bool rendering_ = false;
    bool open_ = false;

};
