
#include <iostream>
#include <string>

#include <networking/networking.h>
#include <utils/log.hpp>
#include <enet/enet.h>

#define LOG_PREFIX "server.cpp::server() : "

namespace Networking
{
	void send_string(const char *string, size_t length, ENetPeer *to) {
		ENetPacket *packet = enet_packet_create(string, length, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(to, 0, packet);
		return;
	}	
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
		
		sserver = enet_host_create(&address, 32, CHANNELS, 0, 0);
		if (sserver == NULL) {
			Logging::ERROR(LOG_PREFIX"Failed to create server.");
			return -1;
		}
		Logging::INFO(LOG_PREFIX"Server have been created successfully.");
		return 0;
	}
	
	void server_loop(void (*connect_callback)(int id), void (*receive_callback)(void *data,int id), void (*disconnect_callback)(int id)) {
		ENetEvent event = {};
		if (enet_host_service(sserver, &event, 10) > 0) {
			switch (event.type) {
				case ENET_EVENT_TYPE_CONNECT:
					Logging::INFO(LOG_PREFIX"New client connected from %d:%u.", event.peer->address.host, event.peer->address.port);
					sclient = event.peer;					
					if (sclient) {
						std::string message = "Hello, client.";
						send_string(message.c_str(), message.size() + 1 , sclient);
					}
					if (connect_callback != NULL)
						connect_callback(event.peer->incomingPeerID);
					break;
				case ENET_EVENT_TYPE_RECEIVE:
					Logging::INFO(LOG_PREFIX"A packet of length %u containg '%s' have been received on channel %u.", 
																		event.packet->dataLength, 
																		event.packet->data, 
																		event.channelID
					);
					if (receive_callback != NULL)
						receive_callback(event.packet->data, event.peer->incomingPeerID);
					enet_packet_destroy(event.packet);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					Logging::INFO(LOG_PREFIX"Client disconnected");
					sclient = 0;
					if (disconnect_callback != NULL)
						disconnect_callback(event.peer->incomingPeerID);
					break;
				case ENET_EVENT_TYPE_NONE:
					break;
			}
		}
		return;
	}
	
	void destroy_server() {
		Logging::INFO("server.cpp::destroy_server : Closing server...");
		enet_host_destroy(sserver);
		return;
	}
	
}

