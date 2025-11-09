
#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <common/enums.hpp>
#include <math/delta.hpp>

#include <backend/glfw_integration.hpp>
#include <utils/resource_manager.hpp>
#include <renderer/renderer.hpp>
#include <backend/backend.hpp>
#include <editor/editor.hpp>
#include <utils/log.hpp>

#include <networking/networking.h>

#include <utils/input.hpp>
#include <application.hpp>

#define SHADER_PATH "../../core/res/shaders/"
#define SERVER 1

namespace BackEnd
{
    
    Shader main_shader;
    
    void force_window_close( ) { GlfwIntegration::force_window_close(); }
    void destroy_application() {
		Logging::INFO("backend.cpp::destroy_application() : Destroying application...");
#if SERVER == 0
		Networking::destroy_server();
#else
		Networking::destroy_client();
#endif
		
		Editor::destroy();
		GlfwIntegration::destroy();
		return;
    }
    
    bool is_window_open() { return GlfwIntegration::is_window_open(); }
    
    int init(const WindowMode& window_mode) {
		if (GlfwIntegration::init(window_mode) == -1) return -1;
#if SERVER == 0
		if (Networking::init_server() == 0)
			Application::add_player();
#else
		if (Networking::init_client() == 0) {
			Application::add_player();
			Application::add_puppet(1);
		}
#endif
		
		ResourceManager::load_shader(&main_shader, "main_shader", SHADER_PATH"object.vert", SHADER_PATH"object.frag");
	
		Gfx::Renderer::init();
		InputManager::init(GlfwIntegration::get_current_window());
		Application::ready();
		
		Editor::init(GlfwIntegration::get_current_window());
		Logging::INFO("backend.cpp::init() : backend initialized successfully.");
		Logging::NOTE("Hello, World!\n");
		return 0;
    }
    
    void begin_frame() {
		GlfwIntegration::begin_frame();
    }
    void end_frame() {
		GlfwIntegration::end_frame();
    }

	static void receive_callback(void *packet, int id) { 
		if (packet != NULL) {
			glm::vec2 *new_position = (glm::vec2*)packet;
			
			Logging::INFO("x: %.1", new_position->x);
			Logging::INFO("y: %.1", new_position->y);
			
			Application::update_puppet_position(*new_position);
		} else
			Logging::ERROR("backend.cpp::receive_callback(void*,int) : Received packet is NULL.");
		return;
	}
	static void connected_callback(int id) {
		Application::add_puppet(id);
		return;
	}
	
    void loop() {
#if SERVER == 0
		Networking::server_loop(connected_callback, NULL, NULL);
#else
		Networking::client_loop(NULL, receive_callback, NULL);
#endif
		begin_frame();
		if (InputManager::is_key_pressed(KEY_ESC)) force_window_close();
		
		Math::Delta::calculate_delta();
		while (Math::Delta::is_frametiming()) {
	    	Application::process(Math::Delta::get_delta_time());
	    	Math::Delta::update();
		}
		render();
	
		end_frame();
    }
    void render() {
		ResourceManager::render_objects();
		ResourceManager::play_animations();
		Editor::render();
		return;
    }
    
}
