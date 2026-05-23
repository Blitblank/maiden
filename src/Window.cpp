
#include "Window.hpp"

#include <SDL3/SDL_events.h>
#include <iostream>

Window::Window() {

    (void)init();

}

Window::~Window() {

    SDL_DestroyWindow(sdlWindow_);
    SDL_Quit();

}

int Window::init() {

    // TODO: config service for controlling window parameters
    sdlWindow_ = SDL_CreateWindow("Maiden", 1280u, 720u, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

    if(sdlWindow_ != nullptr) {
        rendering_ = true;
        open_ = true;
        return 0;
    } else {
        return -1;
    }

}

void Window::handleEvent(SDL_Event& event) {

    if(event.type == SDL_EVENT_QUIT ) {
        open_ = false;
    }

    if(event.type == SDL_EVENT_WINDOW_MINIMIZED) {
        rendering_ = false;
    } else if(event.type == SDL_EVENT_WINDOW_RESTORED) {
        rendering_ = true;
    }
}

bool Window::createSurface(vk::raii::SurfaceKHR* surface) {

    std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] createSurface()" << std::endl;

#ifdef __WIN32
    vk::Win32SurfaceCreateInfoKHR createInfo {
        .hinstance = GetModuleHandle(nullptr),
        .hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(sdlWindow_), "SDL.window.win32.hwnd", nullptr);
    }
    surface = &instance_.createWin32SurfaceKHR(createInfo);
#else // __WIN32

#endif // __WIN32

    std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] attempted to createSurface" << std::endl;
    if(surface != nullptr) {
        return true;
    } else {
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: unable to create window surface." << std::endl;
        return false;
    }
}
