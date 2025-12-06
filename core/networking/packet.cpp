
#include <iostream>
#include <string>

#include <enet/enet.h>

#include <networking/networking.h>

namespace Networking
{
	
	void send_packet(ENetPeer *to, void *data, size_t packet_size, bool reliable) {
		ENetPacket *packet = enet_packet_create(
			data,
			packet_size,
			reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
		);
		enet_peer_send(to, 0, packet);
		return;
	}
	void send_packet_all(ENetHost*host,void*data,size_t packet_size, bool reliable){
		ENetPacket *packet=enet_packet_create(
			data,
			packet_size,
			reliable?ENET_PACKET_FLAG_RELIABLE:ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT
		);
		enet_host_broadcast(host,0,packet);
		return;
	}
	
}

