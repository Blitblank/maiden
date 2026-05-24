
#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan_raii.hpp>

// reference: https://wiki.libsdl.org/SDL3/SDL_CreateWindow
class Window {

public:

    Window();
    ~Window();

    // for SDL3 event polling, runs once per app iteration in its while(running) loop
    void handleEvent(SDL_Event& event);

    bool rendering() { return rendering_; }
    bool open() { return open_; }
    bool createSurface(vk::raii::Instance* instance, vk::raii::SurfaceKHR* surface);

private:

    // launches window, runs at app startup
    int init();

    // this window class will eventually hold mouse, keyboard, audio, etc. interfaces, like an SDL3 hub
    // app will be able to attach callbacks for mouse and keyboard events

    SDL_Window* sdlWindow_;

    bool rendering_ = false;
    bool open_ = false;

};
