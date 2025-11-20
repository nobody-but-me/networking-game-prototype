
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

#include <libs/cJSON.h>

#define SHADER_PATH "../../core/res/shaders/"
int SERVER = 0;

namespace BackEnd
{
    
    Shader main_shader;
 	
	void set_server(int server) { SERVER = server; }
	
    void force_window_close( ) { GlfwIntegration::force_window_close(); }
    void destroy_application() {
		Logging::INFO("backend.cpp::destroy_application() : Destroying application...");
		if (SERVER == 0)
			Networking::destroy_server();
		else
			Networking::destroy_client();
		
		Editor::destroy();
		GlfwIntegration::destroy();
		return;
    }
    
    bool is_window_open() { return GlfwIntegration::is_window_open(); }
    
    int init(const WindowMode& window_mode) {
		if (GlfwIntegration::init(window_mode) == -1) return -1;
		if (SERVER == 0) {
			if (Networking::init_server() == 0)
				Application::add_player();
			else
				return -1;
		}
		else {
			if (Networking::init_client() == 0) {
				Application::add_player();
				Application::add_puppet(1);
			} else
				return -1;
		}
		
		ResourceManager::load_shader(&main_shader, "main_shader", SHADER_PATH"object.vert", SHADER_PATH"object.frag");
	
		Gfx::Renderer::init();
		InputManager::init(GlfwIntegration::get_current_window());
		Application::ready(SERVER);
		
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
//		NOTE: temporary logic to update puppet position
		if (packet != NULL) {
			cJSON *pkt = cJSON_Parse((char*)packet);
			if (pkt == NULL) {
				Logging::ERROR("json packet could not be parsed.");
				return;
			}
			cJSON *x_position = NULL; cJSON *y_position = NULL;
			x_position = cJSON_GetObjectItemCaseSensitive(pkt, "x");
			if (x_position == NULL) {
				Logging::ERROR("could not get X position object item.");
				return;
			}
			y_position = cJSON_GetObjectItemCaseSensitive(pkt, "y");
			if (y_position == NULL){
				Logging::ERROR("could not get Y position object item.");
				return;
			}
			if (!cJSON_IsNumber(x_position) || !cJSON_IsNumber(y_position)) {
				Logging::ERROR("x or y position object item is not number.");
				return;
			}
			glm::vec2 new_puppet_position;
			new_puppet_position.x = x_position->valuedouble;
			new_puppet_position.y = y_position->valuedouble;
			Application::update_puppet_position(new_puppet_position);
		}
		return;
	}
	static void connected_callback(int id) {
		Application::add_puppet(id);
		return;
	}
	
    void loop() {
		if (SERVER == 0)
			Networking::server_loop(connected_callback, receive_callback, NULL);
		else
			Networking::client_loop(NULL, NULL, NULL);
		
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
