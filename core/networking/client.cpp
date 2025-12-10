
#include <iostream>
#include <cstring>

#include <networking/networking.h>
#include <utils/log.hpp>
#include <enet/enet.h>

#include <glm/vec2.hpp>

#define LOG_PREFIX "client.cpp::client() : "

namespace Networking
{
	ENetHost *cclient = NULL;
	ENetPeer *cserver = NULL;
	
	int send_string_to_server(const char*string,int id);
	int send_vec2_to_server(float x, float y,int id) {
		if (cserver == NULL)
			return -1;
		packet_t pkt;
		pkt.payload.data.xf=x;
		pkt.payload.data.yf=y;
		pkt.type=packet_types::vec2_packet;
		pkt.size=sizeof(pkt);
		pkt.id=static_cast<uint8_t>(id);
		send_packet(cserver, &pkt,pkt.size,false);
//		enet_host_flush(cclient); NOTE: should I flush it every time?
		return 0;
	}
		
	int init_client() {
		ENetAddress address = {};
		if (enet_initialize() != 0) {
			Logging::ERROR(LOG_PREFIX"ENet library could not be initialized.");
			return -1;
		}
		cclient = enet_host_create(NULL, 1, CHANNELS, 0, 0);
		if (cclient == NULL) {
			Logging::ERROR(LOG_PREFIX"ENet client could not be created.");
			enet_deinitialize();
			return -1;
		}
		enet_address_set_host(&address, "localhost");
		address.port = 42424;
		
		cserver = enet_host_connect(cclient, &address, CHANNELS, 0);
		if (cserver == NULL) {
			Logging::ERROR(LOG_PREFIX"Failed to connect to server");
			enet_peer_reset(cserver);
			enet_host_destroy(cclient);
			enet_deinitialize();
			return -1;
		}
		Logging::INFO(LOG_PREFIX"Connected successfully");
		return 0;
	}
	
	void client_loop(void (*connect_callback)(int id), void (*receive_callback)(void *data, int id), void (*disconnect_callback)(int id)) {
		ENetEvent event;
		if (enet_host_service(cclient,&event,1)>0) {
			int peer_id=(int)(event.peer-cclient->peers);
			switch(event.type) {
				case ENET_EVENT_TYPE_CONNECT:{
					if (connect_callback!=NULL)
						connect_callback(peer_id);
					break;
					Logging::INFO("connect client id : %d", peer_id);
				}
				case ENET_EVENT_TYPE_RECEIVE:{
					if (receive_callback != NULL)
						receive_callback(event.packet->data,peer_id);
					enet_packet_destroy(event.packet);
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT:{
					Logging::INFO(LOG_PREFIX"server disconnected.");
					if (disconnect_callback!=NULL)
						disconnect_callback(peer_id);
					break;
				}
				case ENET_EVENT_TYPE_NONE:
				default:{
					break;
				}
			}
		}
		return;
	}
	
	void destroy_client() {
		Logging::INFO("client.cpp::destroy_client() : Destroying client...");
		
		enet_peer_reset(cserver);
		enet_host_destroy(cclient);
		enet_deinitialize();
		return;
	}
}
