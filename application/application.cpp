
#include <libs/molson.h>
#include <iostream>
#include <string>
#include <vector>

#include <backend/glfw_integration.hpp>
#include <utils/resource_manager.hpp>
#include <common/object.hpp>
#include <utils/log.hpp>

#include <physics/collision.hpp>
#include <common/object.hpp>
#include <utils/input.hpp>
#include <math/lerp.hpp>

#include <networking/networking.h>

namespace Application {
    
	int SERVER = 0;
	
	Object player, puppet;
	int player_id;
    
    float speed = 4.0f;
	
	void update_puppet_position(glm::vec2 new_position) {
		puppet.position.x = Math::lerp(puppet.position.x, new_position.x, 0.3f);
		puppet.position.y = Math::lerp(puppet.position.y, new_position.y, 0.3f);
//		puppet.position = new_position;
		return;
	}
	
	void add_player() {
		ResourceManager::init_rectangle(&player, "player", nullptr);
		player.colour = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
		player.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		player.position = glm::vec2(0.0f, 0.0f);
		player.scale = glm::vec2(2.0f, 2.0f);
		player.z_index = 1;
		
		player.set_id(0);
	}
	void add_puppet(int id) {
		ResourceManager::init_rectangle(&puppet, "puppet", nullptr);
		puppet.colour = glm::vec4(0.0f, 0.0f, 255.0f, 255.0f);
		puppet.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		puppet.position = glm::vec2(0.0f, 0.0f);
		puppet.scale = glm::vec2(2.0f, 2.0f);
		puppet.z_index = 0;
		
		puppet.set_id(id);
		return;
	}
	
	const double rate = 1.0f / 60.0f;
	double delay = 0;
	void process(double delta) {
		// NOTE: this is going to update the puppet poisition of the connected instance, not the current one
//		if (SERVER!=1) {
			delay+=delta;
			while (delay>=rate){
				SERVER==1?Networking::send_vec2_to_client(player.position.x,player.position.y):
					Networking::send_vec2_to_server(player.position.x,player.position.y);
				delay-=rate;
			}
//		}
		if (player.get_initialized() == true) {
			if (InputManager::is_key_pressed(KEY_D))
				player.position.x += speed * delta;
			if (InputManager::is_key_pressed(KEY_A))
				player.position.x -= speed * delta;
			if (InputManager::is_key_pressed(KEY_S))
				player.position.y -= speed * delta;
			if (InputManager::is_key_pressed(KEY_W))
				player.position.y += speed * delta;
		}
		return;
    }
    void ready(int _SERVER) {
		SERVER = _SERVER;
    }
    
}
