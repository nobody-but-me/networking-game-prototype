
#include <iostream>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include <physics/collision.hpp>
#include <common/object.hpp>
#include <common/enums.hpp>
#include <utils/log.hpp>

namespace Physics
{
    
    bool is_rect_horizontally_aligned(Object *object1, Object *object2);
    bool is_rect_vertically_aligned(Object *object1, Object *object2);
    
    bool is_rect_colliding(Object *object1, Object *object2) {
	if (object1->get_type() != ObjectType::QUAD) {
	    Logging::WARNING("collision.cpp::is_rect_collidng() : Object 1 is not a rectangle.");
	    return false;
	}
	if (object2->get_type() != ObjectType::QUAD) {
	    Logging::WARNING("collision.cpp::is_rect_collidng() : Object 2 is not a rectangle.");
	    return false;
	}
	const float MARGIN = 6.0f;
	
	glm::vec2 center1 = {object1->position.x + object1->scale.x / 2.0f, object1->position.y + object1->scale.y / 2.0f};
	glm::vec2 center2 = {object2->position.x + object2->scale.x / 2.0f, object2->position.y + object2->scale.y / 2.0f};
	
	glm::vec2 distance = {std::abs(center1.x - center2.x), std::abs(center1.y - center2.y)};
	glm::vec2 max = {(object1->scale.x + object2->scale.x) / 2.0f + MARGIN, (object1->scale.y + object2->scale.y) / 2.0f + MARGIN};
	if (distance.x > max.x || distance.y > max.y) return false;
	
	return (object1->position.x < object2->position.x + object2->scale.x    &&
                object1->position.x + object1->scale.x    > object2->position.x &&
		object1->position.y < object2->position.y + object2->scale.y    &&
                object1->position.y + object1->scale.y    > object2->position.y  );
    }
    
}
