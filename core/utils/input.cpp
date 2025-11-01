
#include <GLFW/glfw3.h>
#include <iostream>

namespace InputManager
{
    
    GLFWwindow *window;
    
    bool is_key_pressed(int key) {
	int state = glfwGetKey(window, key);
	if (state == GLFW_PRESS) return true;
	return false;
    }
    
    void init(GLFWwindow *win) {
	window = win;
	return;
    };
    
}
