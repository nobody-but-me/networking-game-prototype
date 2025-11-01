
#pragma once
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <common/animation.hpp>
#include <common/object.hpp>
#include <libs/molson.h>

namespace ResourceManager
{
    
    Animation *get_animation(std::string animation_name);
    Shader *get_shader(std::string shader_program_name);
    Texture *get_texture(std::string texture_name);
    Object *get_object(std::string object_name);
    
    void init_animation(Animation *animation, std::string animation_name, AnimationType animation_type, unsigned int delay, unsigned int init_frame, bool autoplay);
    void load_shader(Shader *shader, const char *shader_name, const char *vertex_shader_path, const char *fragment_shader_path);
    void load_texture(Texture *texture, std::string texture_name, std::string texture_path, bool alpha);
    void init_rectangle(Object *rectangle, std::string object_name, Texture *texture);
    
    void play_animations();
    void render_objects();
    
}


#endif//RESOURCE_MANAGER_HPP
