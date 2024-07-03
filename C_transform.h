/* date = June 25th 2024 11:07 pm */

#ifndef _C_TRANSFORM_H
#define _C_TRANSFORM_H

enum
{
    COLLISION_NONE,
    COLLISION_SAT,
    COLLISION_AABB
};

//Probably should be named C_Collision
struct C_Transform
{
    int entity_id;
    float x;
    float y;
    int w;
    int h;
    int collision_type;
};

//might as well define non entity related transform here
struct Transform
{
    float x;
    float y;
    int w;
    int h;
};


#endif //_C_TRANSFORM_H
