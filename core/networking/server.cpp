
#include <iostream>
#include <cstring>
#include <string>

#include <glm/vec2.hpp>

#include <networking/networking.h>
#include <utils/log.hpp>
#include <enet/enet.h>

#include <common/enums.hpp> //temporary
#include <utils/input.hpp>//temporary

#define LOG_PREFIX "server.cpp::server() : "

namespace Networking
{
	
	ENetHost *sserver = NULL;
	ENetPeer *sclient = NULL;
	
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
		if (InputManager::is_key_pressed(KEY_W)) {
			if (sclient!=NULL) {
				const char *message = "hello, client.";
				ENetPacket *pkt = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(sclient, 0, pkt);
				Logging::INFO("packet sent");
			}
		}
		ENetEvent event = {};
		if (enet_host_service(sserver, &event, 1) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT: {
					Logging::INFO(LOG_PREFIX"New client connected from %d:%u.", event.peer->address.host, event.peer->address.port);
					sclient = event.peer;
					if (sclient == NULL) {
						Logging::FATAL(LOG_PREFIX"could not assign variable to retrive connected client.");
						break;
					}
					if (connect_callback != NULL)
						connect_callback(event.peer->incomingPeerID);
					break;
				}
				case ENET_EVENT_TYPE_RECEIVE: {
					if (receive_callback != NULL)
						receive_callback(event.packet->data, event.peer->incomingPeerID);
					enet_packet_destroy(event.packet);
					break;
				}
				case ENET_EVENT_TYPE_DISCONNECT:
					Logging::INFO(LOG_PREFIX"Client disconnected");
					sclient = 0;
					if (disconnect_callback != NULL)
						disconnect_callback(event.peer->incomingPeerID);
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

