
#pragma once
#ifndef EDITOR_HPP
#define EDITOR_HPP

#include <GLFW/glfw3.h>

namespace Editor
{
    
    void init(GLFWwindow *window);
    void destroy();
    void render();
    
}

#endif//EDITOR_HPP
