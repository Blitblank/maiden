
#include "Window.hpp"

#include <SDL3/SDL_events.h>

Window::Window() {

    (void)init();

}

Window::~Window() {

    SDL_DestroyWindow(sdlWindow_);
    SDL_Quit();

}

int Window::init() {

    // TODO: config service for controlling window parameters
    sdlWindow_ = SDL_CreateWindow("Ouros: Vulkan", 1280u, 720u, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

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
