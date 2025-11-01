
#pragma once
#ifndef GLFW_INTEGRATION_HPP
#define GLFW_INTEGRATION_HPP

#include <iostream>
#include <string>

#include <common/enums.hpp>
#include <GLFW/glfw3.h>

#define  DEFAULT_WINDOW_WIDTH  800
#define  DEFAULT_WINDOW_HEIGHT DEFAULT_WINDOW_WIDTH/4*3
#define DEFAULT_WINDOW_TITLE   "Prototype"

namespace GlfwIntegration
{
    
    void set_window_mode(const WindowMode& window_mode);
    
    unsigned int get_current_window_height();
    unsigned int get_current_window_width();
    GLFWwindow* get_current_window();
    
    bool is_window_minimized();
    bool is_window_focused();
    bool is_window_open();
    
    void toggle_window_fullscreen();
    void force_window_close();
    
    int init(const WindowMode& window_mode);
    void destroy();
    
    void begin_frame();
    void end_frame();
    
}

#endif//GLFW_INTEGRATION_HPP
