
#include <GLFW/glfw3.h>
#include <iostream>

namespace InputManager
{
	
	GLFWwindow *g_window;
	
	// the dumbest way to do it
	bool is_key_pressed(int key) {
		int state = glfwGetKey(g_window, key);
		if (state == GLFW_PRESS)
			return true;
		return false;
	}
    
	void init(GLFWwindow *win) {
		g_window = win;
		return;
	};
    
}
