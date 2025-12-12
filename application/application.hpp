
#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/vec2.hpp>
#include <common/object.hpp>

namespace Application
{
    
	int get_player_id(void);
	int get_puppet_id(uint8_t puppet_id);
	
	void add_player(uint8_t player_id);
	void add_puppet(uint8_t puppet_id);
	
	void update_puppet_position(Object*puppet,glm::vec2 new_position);
	void update_player_position(glm::vec2 new_position);
	
    void process(double delta);
    void ready(int _SERVER);
    
}


#endif//APPLICATION_HPP
