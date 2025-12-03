
#include <iostream>
#include <cstring>
#include <backend/backend.hpp>

#include <utils/log.hpp>
#include <utils/os.hpp>

int main(int argc, char **argv) {
	if (argc > 1) {
		if (strcmp(argv[1], "c") == 0 || strcmp(argv[1], "client") == 0)
			BackEnd::set_server(0);
		else if (strcmp(argv[1], "s") == 0 || strcmp(argv[1], "server") == 0)
			BackEnd::set_server(1);
	}
	if ((BackEnd::init(WindowMode::WINDOWED_MODE)) == -1)
		return -1;
	while (BackEnd::is_window_open())
		BackEnd::loop();
	
	BackEnd::destroy_application();
	return 0;
}
