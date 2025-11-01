
#include <iostream>
#include <string.h>
#include <string>

#include <libs/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <backend/glfw_integration.hpp>
#include <utils/resource_manager.hpp>
#include <renderer/renderer.hpp>
#include <common/object.hpp>
#include <common/enums.hpp>
#include <utils/log.hpp>

#define MOLSON_IMPLEMENTATION
#include <libs/molson.h>

namespace Gfx
{
    
    namespace Renderer
    {
	// TODO: make a better function for that.
	static int check_gl_errors() {
	    GLenum err = glGetError();
	    if ((err == glGetError()) != GL_NO_ERROR) {
		if (err == GL_NO_ERROR) return 0;
		std::string error_message;
		
		if (err == GL_INVALID_FRAMEBUFFER_OPERATION) error_message = "Invalid framebuffer operation.";
		else if (err == GL_INVALID_OPERATION) error_message        = "Invalid operation.";
		else if (err == GL_STACK_UNDERFLOW) error_message          = "stack underflow.";
		else if (err == GL_STACK_OVERFLOW) error_message           = "stack overflow.";
		else if (err == GL_INVALID_VALUE) error_message            = "Invalid value.";
		else if (err == GL_OUT_OF_MEMORY) error_message            = "out of memory.";
		else if (err == GL_INVALID_ENUM) error_message             = "Invalid enum.";
		
		Logging::ERROR("render.cpp::check_gl-errors() : %s.", error_message.c_str());
		return 1;
	    }
	    return 0;
	}
	
	unsigned int g_QUAD_VAO = 0, g_QUAD_VBO = 0;
	Shader *g_main_object_shader;
	
	unsigned int get_quad_vao() { return g_QUAD_VAO; }
	unsigned int get_quad_vbo() { return g_QUAD_VBO; }
	
	int init_global_quad() {
	    glGenVertexArrays(1, &g_QUAD_VAO);
	    glBindVertexArray(g_QUAD_VAO);
	    glGenBuffers(1, &g_QUAD_VBO);
	    
	    glBindBuffer(GL_ARRAY_BUFFER, g_QUAD_VBO);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(RECT_VERTEX_DATA), RECT_VERTEX_DATA, GL_STATIC_DRAW);
	    
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	    glEnableVertexAttribArray(1);
	    
	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    glBindVertexArray(0);
	    return check_gl_errors();
	}
	
	int init_rect(Object *object, Texture *texture, std::string name) {
	    object->set_type(ObjectType::QUAD);
	    object->name = name;
	    
	    if (texture != nullptr) object->set_texture(texture);
	    else                    object->set_texture(nullptr);
	    object->set_initialized(true);
	    return 0;
	}
	
	int set_object_transform(Object *object) {
	    molson(use_shader)(g_main_object_shader);
	    
	    glm::mat4 trans = glm::mat4(1.0f);
	    trans = glm::translate(trans, glm::vec3(object->position.x, object->position.y, object->z_index));
	    
	    trans = glm::translate(trans, glm::vec3( 0.5f * object->scale.x,  0.5f * object->scale.y, 0.0f ));
	    trans = glm::rotate(trans, glm::radians(object->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	    trans = glm::rotate(trans, glm::radians(object->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	    trans = glm::rotate(trans, glm::radians(object->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	    trans = glm::translate(trans, glm::vec3(-0.5f * object->scale.x, -0.5f * object->scale.y, 0.0f ));
	    
	    trans = glm::scale(trans, glm::vec3(object->scale, 1.0f));
	    
	    if (molson(set_matrix4)("transform", &trans, false, g_main_object_shader) != 0) {
		Logging::ERROR("renderer.cpp::set_object_transform() : Failed to set object transform.");
		return -1;
	    }
	    return 0;
	}
	void render_object(Object *object) {
	    Texture *object_texture = object->get_texture();
	    ObjectType object_type = object->get_type();
	    molson(use_shader)(g_main_object_shader);
	    
	    float colour[] = { object->colour.x / 255, object->colour.y / 255, object->colour.z / 255, object->colour.w / 255 };
	    if ((molson(set_vector4_f)("colour", colour, false, g_main_object_shader)) != 0) {
		Logging::ERROR("renderer.cpp::render_object() : Failed to set colour of object %s.", object->name.c_str());
		return;
	    }
	    
	    set_object_transform(object);
	    if (!object_texture) molson(set_bool)("is_textured", false, g_main_object_shader);
	    else {
		molson(set_int)("object_texture", 0, false, g_main_object_shader);
		molson(set_bool)("is_textured", true, g_main_object_shader);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, object_texture->id);
		

		if (object->animated) {
		    molson(set_int)("index", object->animation.get_index(), true, g_main_object_shader);
		    molson(set_int)("columns", object->cols, true, g_main_object_shader);
		    molson(set_int)("rows", object->rows, true, g_main_object_shader);
		    molson(set_bool)("is_animated", true, g_main_object_shader);
		}
	    }
	    
	    switch (object_type)
	    {
		
		case ObjectType::QUAD:
		    glBindVertexArray(g_QUAD_VAO);
		    glDrawArrays(GL_TRIANGLES, 0, 6);
		    glBindVertexArray(0);
		    break;
		case ObjectType::TRIANGLE:
		    break;
		    
		}
		return;
	}
	
	void init() {
	    // molson(init_shader)("main_shader", SHADER_PATH"object.vert", SHADER_PATH"object.frag", &g_main_object_shader);
	    g_main_object_shader = ResourceManager::get_shader("main_shader");
	    
	    glm::mat4 projection = glm::mat4(1.0f);
	    glm::mat4 view = glm::mat4(1.0f);
	    
	    float win_height = static_cast<float>(GlfwIntegration::get_current_window_height()) / 100.0f;
	    float win_width = static_cast<float>(GlfwIntegration::get_current_window_width()) / 100.0f;
	    
	    projection = glm::ortho(win_width * -1.0f, win_width, win_height * -1.0f, win_height, -1.0f, 100.0f);
	    view  = glm::translate(view, glm::vec3(-3.0f, -2.5f, -50.0f)); // TODO: magic numbers.
	    
	    if ((molson(set_matrix4)("projection", &projection, true, g_main_object_shader)) != 0) { Logging::ERROR("renderer.cpp::init() : Failed to set main object shader project uniform variable."); }
	    if ((molson(set_matrix4)("view", &view, true, g_main_object_shader)) != 0) { Logging::ERROR("renderer.cpp::init() : Failed to set main object shader view uniform variable."); }
	    
	    // NOTE: bad: kinda hard-coded; It would be better if, after calling the rect initialize function, the code indentified if the global quad was already loaded or not.
	    init_global_quad();
	    return;
	}
    }
    
}

// Perspective projection alternative
// projection = glm::perspective(glm::radians(45.0f), static_cast<float>(GlfwIntegration::get_current_window_width()) / static_cast<float>(glfwIntegration::get_current_window_height()), 0.1f, 100.0f);
