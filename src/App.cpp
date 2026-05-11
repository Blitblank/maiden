
#include "App.hpp"

#include <thread>
#include <chrono>
#include <SDL3/SDL_events.h>

App::App(): window_(new Window()), engine_(new Engine(window_)) {
    // smart pointers might be a good idea
    // since app will be the top level owner we can use unique ptrs for almost everything
}

int32_t App::run() {

    SDL_Event event;
	while (window_->open()) {
        // app loop for as long as the window is open

        // pass events to the window
        while(SDL_PollEvent(&event) != 0) {
            window_->handleEvent(event);
        }

        // pass vulkan handling to engine
        // call engine.render() or something
        // engine will have a pointer to window so can handle pushing to the screen

        if(window_->rendering()) {
            // engine.draw();

            // TODO: performance profiling :3
            // static int counter = 0;
            // if(counter > 100) {
            //     std::cout << "out" << std::endl;
            //     counter = 0;
            // }
            // counter++;

        } else {
             // throttle while the window isn't active
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // SDL teardown handled in window destructor

    return 0; // maybe want to have an error codes enum for better clarity

}