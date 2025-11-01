
#pragma once
#ifndef LOG_HPP
#define LOG_HPP

#include <string>

namespace Logging
{
    
    // Don't ask me where I got these values.
    namespace Colours {
	enum Colour {
	    YELLOW  = 0x04,
	    MAGENTA = 0x06,
	    GREEN   = 0x03,
	    WHITE   = 0x08,
	    BLACK   = 0x01,
	    NONE    = 0x00,
	    BLUE    = 0x05,
	    CYAN    = 0x07,
	    RED     = 0x02
	};
    }
    
    void WARNING(const char *text, ...);
    void ERROR(const char *text, ...);
    void FATAL(const char *text, ...);
    void NOTE(const char *text, ...);
    void TODO(const char *text, ...);
    void INFO(const char *text, ...);
    void LOG(const char *text, ...);
    
}

#endif//LOG_HPP
