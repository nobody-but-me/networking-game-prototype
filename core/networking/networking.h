
#pragma once
#ifndef NETWORKING_H
#define NETWORKING_H

#include <enet/enet.h>

namespace Networking
{
	constexpr size_t CHANNELS = 2;
	
	void send_string(const char *string, size_t length, ENetPeer *to);	

	void server_loop(void (*connect_callback)(int), void (*receive_callback)(void*, int), void(*disconnect_callback)(int));
	void client_loop(void (*connect_callback)(int), void (*receive_callback)(void*,int), void(*disconnect_callback)(int));
		
	int init_server(void);
	int init_client(void);
	
	void destroy_server(void);
	void destroy_client(void);
}

#endif//NETWORKING_H

