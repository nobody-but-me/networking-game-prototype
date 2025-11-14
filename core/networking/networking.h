
#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <glm/vec2.hpp>

#include <enet/enet.h>


namespace Networking
{
	constexpr size_t CHANNELS = 2;
	
	void send_puppet_position(glm::vec2 new_position); // that's kinda bad
	
	void send_packet(ENetPeer *to, const char *string_data, size_t packet_size, bool reliable);
	void send_packet(ENetPeer *to, void *data, size_t packet_size, bool reliable);
	
	void server_loop(void (*connect_callback)(int), void (*receive_callback)(void*, int), void(*disconnect_callback)(int));
	void client_loop(void (*connect_callback)(int), void (*receive_callback)(void*,int), void(*disconnect_callback)(int));
	
	void destroy_server(void);
	void destroy_client(void);
	
	int init_server(void);
	int init_client(void);
}

#endif//NETWORKING_H

