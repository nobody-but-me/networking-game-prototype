
#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <glm/vec2.hpp>
#include <enet/enet.h>

namespace Networking
{
	enum packet_types: uint8_t
	{
		string_packet=1,
		vec2_packet,
	};
	
	typedef struct pkt_data {
		uint8_t type;
	} pkt_data_t;
	
	typedef struct vec2_packet {
		float x; float y;
		pkt_data_t data;
	} vec2_packet_t;
	typedef struct str_packet {
		char string[256];
		pkt_data_t data;
	}  str_packet_t;
	
	int send_string_to_server(const char*string);
	int send_vec2_to_server(float x, float y);
	
	int send_string_to_client(const char*string);
	int send_vec2_to_client(float x, float y);
	
	constexpr size_t CHANNELS = 2;
	
	void send_packet(ENetPeer *to, void *data, size_t packet_size, bool reliable);
	
	void send_player_position_to_server(glm::vec2 new_player_position);
	
	void server_loop(void (*connect_callback)(int), void (*receive_callback)(void*, int), void(*disconnect_callback)(int));
	void client_loop(void (*connect_callback)(int), void (*receive_callback)(void*,int), void(*disconnect_callback)(int));
	
	void destroy_server(void);
	void destroy_client(void);
	
	int init_server(void);
	int init_client(void);
	
	int init(int server);
	int destroy(void);
}

#endif//NETWORKING_H

