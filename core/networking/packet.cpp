
#include <iostream>
#include <string>

#include <enet/enet.h>

#include <networking/networking.h>
#include <libs/cJSON.h>

namespace Networking
{
	
	void send_packet(ENetPeer *to, const char *string_data, size_t packet_size, bool reliable) {
		ENetPacket *string_packet = enet_packet_create(string_data,
			packet_size,
			reliable ? ENET_PACKET_FLAG_RELIABLE : ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
		enet_peer_send(to, 0, string_packet);
		return;
	}
	
}

