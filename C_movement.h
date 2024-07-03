/* date = June 25th 2024 11:05 pm */

#ifndef _C_MOVEMENT_H
#define _C_MOVEMENT_H

#include <stdbool.h>

enum
{
    MOVETYPE_PLAYER,
    MOVETYPE_COUNT
};


struct C_Movement
{
    int entity_id;
    int type;
    float max_speed;
    float velocity_x;
    float velocity_y;
    float accel; //acceleration
    float decel; //deceleration
    //this should probably be moved to C_State
    int dir_x;
    bool on_ground;
};


void C_Movement_update(struct C_Movement* c, struct Game* game);




#endif //_C_MOVEMENT_H
