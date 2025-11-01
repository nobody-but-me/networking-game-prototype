
#pragma once
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <common/object.hpp>
#include <common/enums.hpp>
#include <libs/molson.h>


constexpr float RECT_VERTEX_DATA[] = {
    0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
    
    0.0f, 1.0f, 1.0f,  0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
    1.0f, 0.0f, 1.0f,  1.0f, 0.0f
};

namespace Gfx
{
    
    namespace Renderer
    {
	
	unsigned int get_quad_vao();
	unsigned int get_quad_vbo();
	
	int init_global_quad();
	
	int init_rect(Object *object, Texture *texture, std::string name);
	
	int set_object_transform(Object *object);
	void render_object(Object *object);
	
	void init();
    }
    
}


#endif//RENDERER_HPP
