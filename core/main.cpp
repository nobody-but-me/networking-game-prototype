
#include <iostream>
#include <cstring>
#include <backend/backend.hpp>

#include <utils/log.hpp>
#include <utils/os.hpp>
#include <libs/cJSON.h>

int main(int argc, char **argv) {
	std::string example = OS::ftos("../../core/example.json");
	const char *string = example.c_str();
//	cJSON *json = cJSON_Parse(string);
	std::cout << string << std::endl;
//	std::cout << cJSON_Print(json) << std::endl;
	
	if (argc > 1) {
	if (strcmp(argv[1], "client") == 0)
		BackEnd::set_server(1);
	else
		BackEnd::set_server(0);
	}
	
    if ((BackEnd::init(WindowMode::WINDOWED_MODE)) == -1) return -1;
    
    while (BackEnd::is_window_open()) BackEnd::loop();
    
    BackEnd::destroy_application();
    return 0;
}
