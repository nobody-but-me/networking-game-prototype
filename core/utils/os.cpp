
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <utils/log.hpp>
#include <utils/os.hpp>

namespace OS
{
	
	std::string ftos(const char *filename) {
		std::stringstream buffer;
		std::fstream file;
		
		file.open(filename, std::ios::in);
		if (!file) {
			Logging::ERROR("os.cpp::ftos() : File could not be opened or found.");
			return NULL;
		}
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	}
	
}

