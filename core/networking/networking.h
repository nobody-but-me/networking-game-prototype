
#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdint.h>

#include <glm/vec2.hpp>
#include <enet/enet.h>

namespace Networking
{
	enum packet_types: uint8_t
	{
		string_packet=1,
		vec2_packet,
	};
	
	typedef struct packet {
		union {
			struct {
				char str[256];
				float xf;
				float yf;
			} data;
		} payload;
		uint32_t size;
		uint8_t type;
		uint8_t id;
	} packet_t;
	
	int send_string_to_server(const char*string);
	int send_vec2_to_server(float x, float y);
	
	int send_string_to_client(const char*string);
	int send_vec2_to_client(float x, float y);
	
	constexpr size_t CHANNELS = 2;
	
	void send_packet(ENetPeer *to, void *data, size_t packet_size, bool reliable);
	void send_packet_all(ENetHost*to,void*data,size_t packet_size,bool reliable);
	
	void send_player_position_to_server(glm::vec2 new_player_position);
	
	void server_loop(void (*connect_callback)(int), void (*receive_callback)(void*, int), void(*disconnect_callback)(int));
	void client_loop(void (*connect_callback)(int), void (*receive_callback)(void*,int), void(*disconnect_callback)(int));
	
	void destroy_server(void);
	void destroy_client(void);
	
	int init_server(void);
	int init_client(void);
	
	int init(int server);
	void loop(void);
	int destroy(void);
}

#endif//NETWORKING_H

