
#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <glm/vec2.hpp>
#include <enet/enet.h>

namespace Networking
{
	enum packet_types: uint8_t
	{
		double_packet = 1,
		integer_packet,
		string_packet,
		vec2_packet,
	};
	
	typedef struct vec2_packet {
		double x;
		double y;
	} vec2_packet_t;
	
	typedef struct packet {
		uint8_t type;
	} packet_t;
	
	int send_string(const char *string);
	int send_vec2(double x, double y);
	int send_double(double value);
	int send_int(int value);
	
	constexpr size_t CHANNELS = 2;
	
	void send_packet(ENetPeer *to, void *data, size_t packet_size, bool reliable);
	
	void send_player_position_to_server(glm::vec2 new_player_position);
	
	void server_loop(void (*connect_callback)(int), void (*receive_callback)(void*, int), void(*disconnect_callback)(int));
	void client_loop(void (*connect_callback)(int), void (*receive_callback)(void*,int), void(*disconnect_callback)(int));
	
	void destroy_server(void);
	void destroy_client(void);
	
	int init_server(void);
	int init_client(void);
}

#endif//NETWORKING_H

