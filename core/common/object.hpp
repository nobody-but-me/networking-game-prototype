
#pragma once
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <common/animation.hpp>
#include <utils/singleton.hpp>
#include <common/enums.hpp>
#include <libs/molson.h>

class Object: public Singleton<Object>
{
    private:
		unsigned int m_indices;
		glm::mat4 m_transform;
		bool m_initialized;
	
		Texture *m_texture;
		ObjectType m_type;
		
		int m_id = 0;
	
	public:
		bool animated = false;
		Animation animation;
		int rows, cols = 1;
	
		int z_index;
	
		glm::vec2 position;
		glm::vec3 rotation;
		glm::vec2 scale;
	
		std::string name;
	
		glm::vec4 colour;
	
		bool selected = false;
	
		void set_initialized(bool new_initialized)	    { this->m_initialized = new_initialized; }
		void set_transform(glm::mat4 new_transform)	    { this->m_transform = new_transform; }
		void set_indices(unsigned int new_indices)	    { this->m_indices = new_indices; }
		void set_texture(Texture *new_texture)		    { this->m_texture = new_texture; }
		void set_type(ObjectType new_type)		        { this->m_type = new_type; }
		void set_id(int new_id)                         { this->m_id = new_id; }
	
		bool get_initialized()		{ return this->m_initialized; }
		glm::mat4 get_transform()	{ return this->m_transform; }
		Texture *get_texture()		{ return this->m_texture; }
		unsigned int get_indices()	{ return this->m_indices; }
		ObjectType get_type()		{ return this->m_type; }
		int get_id()                { return this->m_id; }
	
};

#endif//OBJECT_HPP
