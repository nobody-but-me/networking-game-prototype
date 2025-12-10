
#include <iostream>
#include <cstring>
#include <string>

#include <glm/vec2.hpp>

#include <networking/networking.h>
#include <utils/log.hpp>
#include <enet/enet.h>

#include <common/enums.hpp> //temporary
#include <utils/input.hpp>//temporary

#define LOG_PREFIX "server.cpp::server() : "// im lazy

namespace Networking
{
	
	ENetHost *sserver = NULL;
	ENetPeer *sclient = NULL;
	
	int send_string_to_client(const char*string,bool send_all){
		if(sclient==NULL){
			Logging::ERROR("server.cpp::send_string_to_client(const char*) : sclient is NULL");
			return -1;
		}
		packet_t pkt;
// allocates more memory in case pkt->string is not big enough for string;
		if(strlen(string)>=256){
			size_t new_length=strlen(string+1);
			if((realloc(pkt.payload.data.str,new_length))==NULL){
				Logging::ERROR("server.cpp::send_string_to_client(const char*) : failed to allocate more memory to string array.");
				return -1;
			}
		}
		if((strcpy(pkt.payload.data.str,string))==NULL){
			Logging::ERROR("server.cpp::send_string_to_client(const char*) : copied string for packet is NULL.");
			return -1;
		}
		Logging::INFO("server.cpp::send_string_to_client(cosnt char*) : the follow string is being send to client : `%s`.",
			pkt.payload.data.str);
		pkt.type=packet_types::string_packet;
		pkt.size=sizeof(pkt);
		send_all==true?send_packet_all(sserver,&pkt,pkt.size,false):send_packet(sclient,&pkt,pkt.size,false);
		return 0;
	}
	int send_vec2_to_client(float x,float y,bool send_all) {
		if (sclient==NULL)
			return -1;
		packet_t pkt;
		pkt.payload.data.xf=x;
		pkt.payload.data.yf=y;
		pkt.type=packet_types::vec2_packet;
		pkt.size=sizeof(pkt);
		
		send_all==true?send_packet_all(sserver,&pkt,pkt.size,false):send_packet(sclient,&pkt,pkt.size,false);
		return 0;
	}
	int send_int_to_client(int value,bool send_all){
		if(sclient==NULL)
			return -1;
		packet_t pkt;
		pkt.payload.data.xi=value;
		pkt.type=packet_types::int_packet;
		pkt.size=sizeof(pkt);
		send_all==true?send_packet_all(sserver,&pkt,pkt.size,false):send_packet(sclient,&pkt,pkt.size,false);
		return 0;
	}
	
	int init_server() {
		ENetAddress address = {};
		if (enet_initialize() != 0) {
			Logging::ERROR(LOG_PREFIX"ENet library could not be initialized.");
			return -1;
		}
		address.host = ENET_HOST_ANY;
		address.port = 42424; // ENET_PORT_ANY;
		
		sserver = enet_host_create(&address, 5, CHANNELS, 0, 0);
		if (sserver == NULL) {
			Logging::ERROR(LOG_PREFIX"Failed to create server.");
			enet_deinitialize();
			return -1;
		}
		Logging::INFO(LOG_PREFIX"Server have been created successfully.");
		return 0;
	}
	
	void server_loop(void (*connect_callback)(int id), void (*receive_callback)(void *data,int id), void (*disconnect_callback)(int id)) {
		if (InputManager::is_key_pressed(KEY_ENTER)) {
			if (sclient!=NULL) {
				const char*message="hello, client";
				send_string_to_client(message,false);
				Logging::INFO("packet had been sent.");
			}
		}
		ENetEvent event = {};
		if (enet_host_service(sserver, &event, 1) > 0) {
			int peer_id=(int)((event.peer-sserver->peers)+1);
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT: {
					Logging::INFO(LOG_PREFIX"New client connected from %d:%u.", event.peer->address.host, event.peer->address.port);
					sclient = event.peer;
					if (sclient==NULL){
						Logging::FATAL(LOG_PREFIX"could not assign variable to retrive connected client.");
						break;
					}
					if (connect_callback != NULL)
						connect_callback(peer_id);
					Logging::INFO("connect server id : %d", peer_id);
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE: {
// event.peer->incomingPeerID;
					if (receive_callback != NULL)
						receive_callback(event.packet->data,peer_id);
					enet_packet_destroy(event.packet);
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT:
					Logging::INFO(LOG_PREFIX"Client disconnected");
					sclient = 0;
					if (disconnect_callback != NULL)
						disconnect_callback(peer_id);
					break;
				case ENET_EVENT_TYPE_NONE:
				default:
					break;

			}
		}
		return;
	}
	
	void destroy_server() {
		Logging::INFO("server.cpp::destroy_server : Closing server...");
		enet_host_destroy(sserver);
		enet_deinitialize();
		return;
	}
	
}

