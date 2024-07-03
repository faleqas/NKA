/* date = June 27th 2024 10:55 pm */

#ifndef SAT_H
#define SAT_H
#include "component.h"
#include <stdbool.h>

struct Point
{
    float x;
    float y;
};


//Seperating axis theorem algorithm
bool is_colliding_sat(const struct C_Transform* trans1,
                      const struct C_Transform* trans2);

bool is_colliding_aabb(const struct C_Transform* trans1,
                      const struct C_Transform* trans2);

float dot_product(float x1, float x2,
                  float y1, float y2);

#endif //SAT_H
