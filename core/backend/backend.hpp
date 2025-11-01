
#pragma once
#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <common/enums.hpp>

namespace BackEnd
{
    
    void force_window_close( );
    void destroy_application();
    
    bool is_window_open();
    
    int init(const WindowMode& window_mode);
    
    void begin_frame();
    void end_frame();
    
    void render();
    void loop();
    
}


#endif//BACKEND_HPP
