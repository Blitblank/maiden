
#include "Window.hpp"

#include <SDL3/SDL_events.h>
#include <iostream>

Window::Window(Logger* logger) : logger_(logger) {

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

bool Window::createSurface(vk::raii::Instance* instance, vk::raii::SurfaceKHR* surface) {

    if(instance == nullptr) {
<<<<<<< HEAD
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: cannot create surface with a null Vulkan instance." << std::endl;
=======
        logger_->log("Window", LogFlag::Error, "Cannot create surface with a null Vulkan instance.");
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d
        return false;
    }

#ifdef __WIN32
    vk::Win32SurfaceCreateInfoKHR createInfo {
        .hinstance = GetModuleHandle(nullptr),
        .hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(sdlWindow_), "SDL.window.win32.hwnd", nullptr);
    }
    surface = instance->createWin32SurfaceKHR(createInfo);
#else // linux (this technically works on windows too but vulkan gives us an explicit method for WIN32)
    // its just sdl3 uses the c vulkan api and the app uses the c++ api
    VkSurfaceKHR cSurface;
    (void)SDL_Vulkan_CreateSurface(sdlWindow_, static_cast<VkInstance>(**instance), nullptr, &cSurface);
    *surface = vk::raii::SurfaceKHR(*instance, cSurface);
#endif // __WIN32

    if(surface != nullptr) {
        return true;
    } else {
<<<<<<< HEAD
        std::cout << "[" << __FUNCTION__ << ": " << __LINE__ << "] Error: unable to create window surface." << std::endl;
=======
        logger_->log("Window", LogFlag::Error, "Unable to create window surface.");
>>>>>>> 01992b54c3fd6f2627cdee9be7f7b2703cd7957d
        return false;
    }
}

bool Window::getExtent(int32_t* width, int32_t* height) {
    SDL_GetWindowSizeInPixels(sdlWindow_, width, height);
    return true;
}
