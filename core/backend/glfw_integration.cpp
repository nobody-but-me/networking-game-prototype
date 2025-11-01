
#include <iostream>
#include <string>

#include <glad.h>
#include <GLFW/glfw3.h>

#include <backend/glfw_integration.hpp>
#include <common/enums.hpp>
#include <utils/log.hpp>

namespace GlfwIntegration
{
    
    unsigned int g_current_window_height = 0;
    unsigned int g_current_window_width  = 0;
    unsigned int g_fullscreen_height     = 0;
    unsigned int g_fullscreen_width      = 0;
    unsigned int g_windowed_height       = 0;
    unsigned int g_windowed_width        = 0;
    
    bool g_force_window_close = false;
    bool g_window_focused = true;
    
    WindowMode g_window_mode;
    
    const GLFWvidmode *g_mode;
    
    GLFWwindow  *g_window = NULL;
    GLFWmonitor *g_monitor;
    
    void set_window_mode(const WindowMode& window_mode) {
	g_window_mode = window_mode;
	if (window_mode == WindowMode::WINDOWED_MODE) {
	    g_current_window_height = g_windowed_height;
	    g_current_window_width = g_windowed_width;
	}
	else if (window_mode == WindowMode::FULLSCREEN_MODE) {
	    g_current_window_height = g_fullscreen_height;
	    g_current_window_width = g_fullscreen_width;
	}
	glfwSetWindowMonitor(g_window, NULL, 0, 0, g_current_window_width, g_current_window_height, g_mode->refreshRate);
	glfwSetWindowPos(g_window, 0, 0);
	return;
    }
    
    unsigned int get_current_window_height() { return g_current_window_height; }
    unsigned int get_current_window_width() { return g_current_window_width; }
    GLFWwindow* get_current_window() { return g_window; }
    
    bool is_window_minimized();
    bool is_window_focused();
    bool is_window_open() { return !(glfwWindowShouldClose(g_window) || g_force_window_close); }
    
    void toggle_window_fullscreen() {
	if (g_window_mode == WindowMode::WINDOWED_MODE) set_window_mode(WindowMode::FULLSCREEN_MODE);
	else                                            set_window_mode(WindowMode::WINDOWED_MODE);
	return;
    }
    void force_window_close() { g_force_window_close = true; return; }
    
    static void glfw_error_callback(int error, const char *description) {
	Logging::ERROR("GLFW Error : \n [ %s ]\n DESCRITION: %s.", std::to_string(error).c_str(), description);
	return;
    }
    int init(const WindowMode& window_mode) {
	glfwInit();
	glfwSetErrorCallback(glfw_error_callback);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_VISIBLE  , GLFW_TRUE );
	
	g_monitor = glfwGetPrimaryMonitor( );
	g_mode = glfwGetVideoMode(g_monitor);
	
	glfwWindowHint(GLFW_REFRESH_RATE, g_mode->refreshRate);
	glfwWindowHint(GLFW_GREEN_BITS  , g_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS   , g_mode->blueBits);
	glfwWindowHint(GLFW_RED_BITS    , g_mode->redBits);
	
	g_fullscreen_height = g_mode->height;
	g_fullscreen_width = g_mode->width;
	
	g_windowed_height = DEFAULT_WINDOW_HEIGHT;
	g_windowed_width = DEFAULT_WINDOW_WIDTH;
	
	g_window_mode = window_mode; 
	if (g_window_mode == WindowMode::WINDOWED_MODE) {
	    g_current_window_height = g_windowed_height;
	    g_current_window_width = g_windowed_width;
	    
	    g_window = glfwCreateWindow(g_windowed_width, g_windowed_height, DEFAULT_WINDOW_TITLE, NULL, NULL);
	    glfwSetWindowPos(g_window, 0, 0);
	}
	else if (g_window_mode == WindowMode::FULLSCREEN_MODE) {
	    g_current_window_height = g_fullscreen_height;
	    g_current_window_width = g_fullscreen_width;
	    
	    g_window = glfwCreateWindow(g_windowed_width, g_windowed_height, DEFAULT_WINDOW_TITLE, g_monitor, NULL);
	}
	
	if (g_window == NULL) {
	    Logging::ERROR("glfw_integration.cpp::init() : Failed to initialize window.");
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(g_window);
	int glad_version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if (!glad_version) {
	    Logging::ERROR("glfw_integration.cpp::init() : failed to initialize OpenGL context. The application is not glad at all.");
	    glfwTerminate();
	    return -1;
	}
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE); // NOTE: alpha to coverage method to render transparency. Study depth peeling later.
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	
	Logging::INFO("glfw_integration.cpp::init() : Window had been configured successfully.");
	return 0;
    }
    void destroy() {
	if (g_window_mode == WindowMode::FULLSCREEN_MODE) toggle_window_fullscreen();
	glfwTerminate();
	return;
    }
    
    void begin_frame() {
	glfwPollEvents();
	glClearColor(0.07f, 0.07f, 0.07f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	
	int focused = glfwGetWindowAttrib(g_window, GLFW_FOCUSED);
	g_window_focused = (focused != 0);
	return;
    }
    void end_frame() {
	glfwSwapBuffers(g_window);
	return;
    }
}
