
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#include <libs/glad.h>
#include <GLFW/glfw3.h>

#include <utils/resource_manager.hpp>
#include <renderer/renderer.hpp>
#include <common/animation.hpp>
#include <common/object.hpp>
#include <libs/molson.h>
#include <utils/log.hpp>

namespace ResourceManager
{
    
    std::vector<Animation *> g_animations;
    std::vector<Texture *> g_textures;
    std::vector<Shader *> g_shaders;
    std::vector<Object *> g_objects;
    
    Animation *get_animation(std::string animation_name) {
	auto animation = std::find_if(g_animations.begin(), g_animations.end(), [&](const Animation *anim) { return anim->name == animation_name; });
	if (animation != g_animations.end()) return *animation;
	else return NULL;
    }
    Texture *get_texture(std::string texture_name) {
	auto texture = std::find_if(g_textures.begin(), g_textures.end(), [&](const Texture *tex) { return tex->name == texture_name; });
	if (texture != g_textures.end()) return *texture;
	else return NULL;
    }
    Shader *get_shader(std::string shader_program_name) {
	auto shader = std::find_if(g_shaders.begin(), g_shaders.end(), [&](const Shader *shader) { return shader->name == shader_program_name; });
	if (shader != g_shaders.end()) return *shader;
	else return NULL;

    }
    Object *get_object(std::string object_name) {
	auto object = std::find_if(g_objects.begin(), g_objects.end(), [&](const Object *obj) { return obj->name == object_name; });
	if (object != g_objects.end()) { 
	    Logging::INFO("resource_manager.cpp::get_object() : Object had been found successfully");
	    return *object;
	} else {
	    Logging::ERROR("resource_manager.cpp::get_object() : Object not found.");
	    return NULL;
	}
    }
    
    // TODO: ensure that there are not already other objects with the same name in the vectors.
    void init_animation(Animation *animation, std::string animation_name, AnimationType animation_type, unsigned int delay, unsigned int init_frame, bool autoplay) {
	animation->name = animation_name;
	
	animation->set_animation_type(animation_type);
	animation->set_autoplay(autoplay);
	animation->set_index(init_frame);
	animation->set_delay(delay);
	
	if (autoplay == true) animation->set_is_playing(true);
	g_animations.emplace_back(animation);
	return;
    }
    void load_texture(Texture *texture, std::string texture_name, std::string texture_path, bool alpha = true) {
	if (texture_path == "") return;
	*texture = molson(load_texture)(texture_name.c_str(), texture_path.c_str(), alpha);
	g_textures.emplace_back(texture);
	return;
    }
    void load_shader(Shader *shader, const char *shader_name, const char *vertex_shader_path, const char *fragment_shader_path) {
	if ((strcmp("", fragment_shader_path)) == 0) return;
	if ((strcmp("", vertex_shader_path)) == 0) return;
	molson(init_shader)(shader_name, vertex_shader_path, fragment_shader_path, shader);
	g_shaders.emplace_back(shader);
	return;
    }
    
    void init_rectangle(Object *rectangle, std::string object_name, Texture *texture) {
	Gfx::Renderer::init_rect(rectangle, texture, object_name);
	g_objects.emplace_back(rectangle);
	return;
    }
    
    void play_animations() {
	for (Animation *anim : g_animations) {
	    if (anim->get_is_playing()) anim->process();
	    else                        anim->stop();
	}
	return;
    }
    
    void render_objects() {
	for (Object *obj : g_objects) {
	    Gfx::Renderer::render_object(obj);
	}
	return;
    }
    
}
