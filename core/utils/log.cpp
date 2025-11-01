
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <utils/log.hpp>

namespace Logging
{
    
    static inline std::string set_colour(Colours::Colour foreground = Colours::Colour::NONE, Colours::Colour background = Colours::Colour::NONE) {
	std::stringstream s; s << "\033[";
	if (!foreground && ! background) s << "0";
	if (foreground) {
	    s << 29 + foreground;
	    if (background) s << ";";
	}
	if (background) s << 39 + background; s << "m";
	return s.str();
    }
    
    static void MESSAGE(Colours::Colour foreground_colour = Colours::Colour::NONE, Colours::Colour background_colour = Colours::Colour::NONE, const char *string = "", va_list args = NULL) {
	char token[1000]; int index = 0;
	for (int i = 0; string[i] != '\0'; i++) {
	    token[index++] = string[i];
	    if (string[i + 1] == '%' || string[i + 1] == '\0') {
		token[index] = '\0'; index = 0;
		if (token[0] != '%') fprintf(stdout, "%s%s", set_colour(foreground_colour, background_colour).c_str(), token);
		else {
		    char character_type = 0;
		    int j = 1;
		    
		    while ((character_type = token[j++]) < 58) {}
		    if (character_type == 'i' || character_type == 'd' || character_type == 'u' || character_type == 'h') fprintf(stdout, token, va_arg(args, int));
		    else if (character_type == 'f') fprintf(stdout, token, va_arg(args, double));
		    else if (character_type == 'c') fprintf(stdout, token, va_arg(args, int));
		    else if (character_type == 'l') {
			char ct2 = token[2];
			
			if (ct2 == 'u' || ct2 == 'd' || ct2 == 'i') fprintf(stdout, token, va_arg(args, long));
			else if (ct2 == 'f') fprintf(stdout, token, va_arg(args, double));
		    }
		    else if (character_type == 'L') {
			char ct2 = token[2];
			
			if (ct2 == 'u' || ct2 == 'd'|| ct2 == 'i') fprintf(stdout, token, va_arg(args, long long));
			else if (ct2 == 'f') fprintf(stdout, token, va_arg(args, long double));
		    }
		    else if (character_type == 's') fprintf(stdout, token, va_arg(args, char*));
		    else fprintf(stdout, "%s", token);
		}
	    }
	}
	fprintf(stdout, "\n%s", set_colour().c_str());
	return;
    }
    
    void WARNING(const char *text, ...) {
	const char *warning = "[WARNING] : ";
	va_list args; va_start(args, text);
	
	size_t length = strlen(warning) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", warning, text);
	
	MESSAGE(Colours::Colour::YELLOW, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
    void ERROR(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *error = "[ERROR] : ";
	
	size_t length = strlen(error) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", error, text);
	
	MESSAGE(Colours::Colour::RED, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
    void FATAL(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *fatal = "[FATAL] : ";
	
	size_t length = strlen(fatal) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", fatal, text);
	
	MESSAGE(Colours::Colour::BLACK, Colours::Colour::RED, new_string, args);
	va_end(args);
	return;
    }
    void NOTE(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *note = "[NOTE] : ";
	
	size_t length = strlen(note) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", note, text);
	
	MESSAGE(Colours::Colour::WHITE, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
    void TODO(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *todo = "[TODO] : ";
	
	size_t length = strlen(todo) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", todo, text);
	
	MESSAGE(Colours::Colour::MAGENTA, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
    void INFO(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *info = "[INFO] : ";
	
	size_t length = strlen(info) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", info, text);
	
	MESSAGE(Colours::Colour::BLACK, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
    void LOG(const char *text, ...) {
	va_list args; va_start(args, text);
	const char *log = "[LOG] : ";
	
	size_t length = strlen(log) + strlen(text) + 1;
	char *new_string = (char*)malloc(length);
	snprintf(new_string, length, "%s%s", log, text);
	
	MESSAGE(Colours::Colour::CYAN, Colours::Colour::NONE, new_string, args);
	va_end(args);
	return;
    }
}
