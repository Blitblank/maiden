
#include "App.hpp"

void App::run() {

    std::cout << "im an app and im running !!" << std::endl;

    (void)foo();

    return;

}

int32_t App::foo() {

    return 12;

}
