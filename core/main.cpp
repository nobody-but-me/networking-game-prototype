
#include <iostream>
#include <backend/backend.hpp>

int main(int argc, char *argv[]) {
    if ((BackEnd::init(WindowMode::WINDOWED_MODE)) == -1) return -1;
    
    while (BackEnd::is_window_open()) BackEnd::loop();
    
    BackEnd::destroy_application();
    return 0;
}
