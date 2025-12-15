
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#include <libs/molson.h>

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
	
	std::vector<Object> puppets;
	Object player;//, puppet;
	int player_id;
    
    float speed = 4.0f;
	
	int get_player_id(void){
		if(player.get_initialized()==false)
			return -1;
		return player.get_id();
	}
	int get_puppet_id(uint8_t puppet_id){
		
//		if(puppet.get_initialized()==false)
//			return -1;
//		return  puppet.get_id();
		return -1;
	}
	
	void update_puppet_position(Object*puppet,glm::vec2 new_position) {
		puppet->position.x=Math::lerp(puppet->position.x,new_position.x,0.3f);
		puppet->position.y=Math::lerp(puppet->position.y,new_position.y,0.3f);
//		puppet.position = new_position;
		return;
	}
	
	void add_player(uint8_t player_id) {
		ResourceManager::init_rectangle(&player, "player", nullptr);
		player.colour = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
		player.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		player.position = glm::vec2(0.0f, 0.0f);
		player.scale = glm::vec2(2.0f, 2.0f);
		player.z_index = 1;
		
		player.set_id(static_cast<uint8_t>(player_id));
	}
	void add_puppet(uint8_t puppet_id) {
		std::string name_id=std::to_string(puppet_id);
		std::string base_name="puppet";
		std::string final_name=base_name+name_id; // it looks even false, like it should not be that easy;
		Logging::INFO("new puppet name: %s.",final_name.c_str());
		
		Object new_puppet;
		puppets.emplace_back(new_puppet);
		int idx=(puppets.size()-1);
		
		ResourceManager::init_rectangle(&puppets[idx],final_name.c_str(),nullptr);
		puppets[idx].colour=glm::vec4(0.0f,0.0f,255.0f,255.0f);
		puppets[idx].rotation=glm::vec3(0.0f,0.0f,0.0f);
		puppets[idx].position=glm::vec2(0.0f,0.0f);
		puppets[idx].scale=glm::vec2(2.0f,2.0f);
		puppets[idx].z_index=0;
		
		puppets[idx].set_id(static_cast<int>(puppet_id));
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
