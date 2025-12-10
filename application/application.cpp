
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
	
	int get_player_id(void){
		if(player.get_initialized()==false)
			return -1;
		return player.get_id();
	}
	int get_puppet_id(void){
		if(puppet.get_initialized()==false)
			return -1;
		return  puppet.get_id();
	}
	
	void update_puppet_position(glm::vec2 new_position) {
		puppet.position.x = Math::lerp(puppet.position.x, new_position.x, 0.3f);
		puppet.position.y = Math::lerp(puppet.position.y, new_position.y, 0.3f);
//		puppet.position = new_position;
		return;
	}
	
	void add_player(int id) {
		ResourceManager::init_rectangle(&player, "player", nullptr);
		player.colour = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
		player.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		player.position = glm::vec2(0.0f, 0.0f);
		player.scale = glm::vec2(2.0f, 2.0f);
		player.z_index = 1;
		
		player.set_id(id);
	}
	void add_puppet(int id) {
		ResourceManager::init_rectangle(&puppet, "puppet", nullptr);
		puppet.colour = glm::vec4(0.0f, 0.0f, 255.0f, 255.0f);
		puppet.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		puppet.position = glm::vec2(0.0f, 0.0f);
		puppet.scale = glm::vec2(2.0f, 2.0f);
		puppet.z_index = 0;
		
		puppet.set_id(id);
		Logging::INFO("puppet id : %d",id);
		return;
	}
	
	glm::vec2 last_pos=player.position;
	
	const double rate=1.0f/60.0f; // r-(h)ate;
	double delay=0;
	void process(double delta) {
		
// updating player position to the server or to the client at a fixed framerate;
		if (player.get_initialized()==true){
			if(last_pos!=player.position){
				delay+=delta;
				while (delay>=rate){
					SERVER==1?Networking::send_vec2_to_client(player.position.x,player.position.y,player.get_id(),true):
						Networking::send_vec2_to_server(player.position.x,player.position.y,player.get_id());
					delay-=rate;
				}
			}

// TODO: encapsulate this logic in a separte function? Maybe;
// updating player position;
			if (InputManager::is_key_pressed(KEY_D))
				player.position.x += speed * delta;
			else if (InputManager::is_key_pressed(KEY_A))
				player.position.x -= speed * delta;
			else if (InputManager::is_key_pressed(KEY_S))
				player.position.y -= speed * delta;
			else if (InputManager::is_key_pressed(KEY_W))
				player.position.y += speed * delta;
			else
				last_pos=player.position;
			}
		return;
    }
    void ready(int _SERVER) {
		SERVER = _SERVER;
    }
    
}
