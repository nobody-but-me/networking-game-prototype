
#pragma once
#ifndef COLLISION_HPP
#define COLLISION_HPP

#include <common/object.hpp>

namespace Physics
{
    
    bool is_rect_horizontally_aligned(Object *object1, Object *object2);
    bool is_rect_vertically_aligned(Object *object1, Object *object2);
    
    bool is_rect_colliding(Object *object1, Object *object2);
    
}


#endif//COLLISION_HPP
