
#include <iostream>
#include <string>

#include <networking/networking.h>
#include <utils/input.hpp>
#include <utils/log.hpp>

#include <glm/vec2.hpp>

#include <application.hpp>
#include <utils/resource_manager.hpp>

namespace Networking
{
	int IS_SERVER = 0;
	
	bool is_server() {
		return IS_SERVER;
	}
	
	int init(int is_server) {
		IS_SERVER = is_server;
		is_server == 1 ? Networking::init_server() : Networking::init_client();
		
// adding the server player if the current application is the server
// adding the server puppet if the current application is the client
		if(is_server==1)
			Application::add_player(0);
		else
			Application::add_puppet(0);
		return 0;
	}
	
	static void connected_callback(int id){
		if(IS_SERVER==1){
			Application::add_puppet(id);
			Networking::send_int_to_client(id, false);
		}
		return;
	}
	static void received_callback(void*packet,int id){
		Networking::packet_t*pkt=reinterpret_cast<Networking::packet_t*>(packet);
		if(pkt==NULL){
			Logging::ERROR("networking.cpp::received_callback(void*,int):reinterpret base packet  is NULL.");
			return;
		}
		uint8_t pkt_id=pkt->id+1;
		switch(pkt->type){
			case Networking::packet_types::string_packet:{
//				Logging::INFO("networking.cpp::received_callback(void*,int) : received string: '%s'",string_data->string);
				break;
			}
			case Networking::packet_types::vec2_packet:{
// NOTE: later, it'll iterate a list with every puppet in the scene to find the correct one;
				std::string base_name="puppet";
				std::string str_id=std::to_string(pkt_id);
				std::string puppet_name=base_name+str_id;
				
//				Logging::INFO("name:%s",puppet_name.c_str());
				
				Object*puppet=ResourceManager::get_object(puppet_name);
				if(puppet==NULL)
					break;
				float x=pkt->payload.data.xf;
				float y=pkt->payload.data.yf;
				glm::vec2 new_pos=glm::vec2(x,y);
				Application::update_puppet_position(puppet,new_pos);
				break;
			}
			case Networking::packet_types::int_packet:{
				if(IS_SERVER==0){
					uint8_t value=pkt->payload.data.xi;
					Application::add_player(value);
					break;
				}
			}
			default:{
				break;
			}
		}
		return;
	}
	
	void loop(void){
		IS_SERVER==1?Networking::server_loop(connected_callback,received_callback,NULL):
			Networking::client_loop(NULL,received_callback,NULL);
		return;
	}
	int destroy(void) {
		IS_SERVER == 1 ? Networking::destroy_server() : Networking::destroy_client();
		return 0;
	}
	
}

