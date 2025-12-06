
#pragma once
#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>

namespace InputManager
{
    
    bool is_key_pressed(int key);
    void init(GLFWwindow *win);
    
}

#endif//INPUT_HPP
