
#include <iostream>

// NOTE: perhaps it would be better to retrive glfw values from the backend.
#include <libs/glad.h>
#include <GLFW/glfw3.h>

#include <math/delta.hpp>

constexpr double MAX_FRAMERATE = 60.0f;

namespace Math
{
    namespace Delta
    {
	
	double previous_time = 0.0f;
	double current_time = 0.0f;
	double delta = 1 / MAX_FRAMERATE;
	double updates = 0.0f;
	double time = 0.0f;
	double frame_time;
	
	bool is_frametiming() {
	    return (frame_time > 0.0f);
	}
	
	float get_delta_time() {
	    return std::min(frame_time, delta);
	}
	
	void update() {
	    float dt = Math::Delta::get_delta_time();
	    frame_time -= dt;
	    updates += dt;
	    return;
	}
	
	void calculate_delta() {
	    previous_time = current_time;
	    current_time = glfwGetTime();
	    frame_time = current_time - previous_time;
	    return;
	}
	
    }
}
