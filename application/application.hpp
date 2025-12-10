
#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glm/vec2.hpp>

namespace Application
{
    
	int get_player_id(void);
	int get_puppet_id(void);
	
	void add_player(int id);
	void add_puppet(int id);
	
	void update_puppet_position(glm::vec2 new_position);
	void update_player_position(glm::vec2 new_position);
	
    void process(double delta);
    void ready(int _SERVER);
    
}


#endif//APPLICATION_HPP
