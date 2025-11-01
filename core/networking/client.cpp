

#include <iostream>
#include <string>

#include <networking/networking.h>
#include <utils/log.hpp>
#include <enet/enet.h>

#define LOG_PREFIX "client.cpp::client() : "

namespace Networking
{
	ENetHost *cclient = NULL;
	ENetPeer *cserver = NULL;
	
	int init_client() {
		ENetAddress address = {};
		if (enet_initialize() != 0) {
			Logging::ERROR(LOG_PREFIX"ENet library could not be initialized.");
			return -1;
		}
		cclient = enet_host_create(NULL, 1, CHANNELS, 0, 0);
		if (cclient == NULL) {
			Logging::ERROR(LOG_PREFIX"ENet client could not be created.");
			return -1;
		}
		enet_address_set_host(&address, "localhost");
		address.port = 42424;
		
		cserver = enet_host_connect(cclient, &address, CHANNELS, 0);
		if (cserver == NULL) {
			Logging::ERROR(LOG_PREFIX"Failed to connect to server");
			enet_peer_reset(cserver);
			enet_host_destroy(cclient);
			return -1;
		}
		Logging::INFO(LOG_PREFIX"Connected successfully");
		return 0;
	}
	
	void client_loop(void (*connect_callback)(int id), void (*receive_callback)(void *data, int id), void (*disconnect_callback)(int id)) {
		ENetEvent event = {};
		if (enet_host_service(cclient, &event, 0) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
			Logging::INFO(LOG_PREFIX"Connect successfully.");
			ENetEvent event;
			if (enet_host_service(cclient, &event, 10) > 0) {
				switch (event.type) {
					case ENET_EVENT_TYPE_CONNECT:
						if (connect_callback != NULL)
							connect_callback(event.peer->incomingPeerID);
						break;
					case ENET_EVENT_TYPE_NONE: break;
					
					case ENET_EVENT_TYPE_RECEIVE:
						Logging::INFO(LOG_PREFIX"A packet of length %u containing '%s' have been received on channel %u.", 
														event.packet->dataLength, 
														event.packet->data, 
														event.channelID
						);
						enet_packet_destroy(event.packet);
						if (receive_callback != NULL)
							receive_callback(event.peer->data, event.peer->incomingPeerID);
						break;
					case ENET_EVENT_TYPE_DISCONNECT:
						Logging::INFO(LOG_PREFIX"Server disconnected.");
						if (disconnect_callback != NULL)
							disconnect_callback(event.peer->incomingPeerID);
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
		return;
	}
}
