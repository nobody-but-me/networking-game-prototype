

#include <iostream>

#include <networking/networking.h>


namespace Networking
{
	int IS_SERVER = 0;
	
	bool is_server() {
		return IS_SERVER;
	}
	
	int init(int server) {
		IS_SERVER = server;
		server == 1 ? Networking::init_server() : Networking::init_client();
		return 0;
	}
	int destroy(void) {
		IS_SERVER == 1 ? Networking::destroy_server() : Networking::destroy_client();
		return 0;
	}
	
}
