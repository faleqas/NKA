/* date = June 25th 2024 11:05 pm */

#ifndef _C_MOVEMENT_H
#define _C_MOVEMENT_H

#include <stdbool.h>

enum
{
    MOVETYPE_DUMMY,
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
    
    int dir_x;
    
    //TODO(omar): turn these into bitwise flags of a single flags variable
    bool on_ground;
    bool on_wall;
    //true if a collision with either direction occured this tic
    bool colliding_x;
    bool colliding_y;
    
    
    int last_collision_x; //direction of last collision in X axis
};


void C_Movement_update(struct C_Movement* c, struct Game* game);


void C_Movement_halt_to_stop(struct C_Movement* c, struct Game* game, float decel_factor);

void C_Movement_flip_dir(struct C_Movement* c);


//if the distance between the 2 transforms is less than the dead zone then the direction
//will be 0
void C_Movement_face_target(struct C_Movement* c,
                            const struct C_Transform* trans,
                            const struct C_Transform* target,
                            int dead_zone);


#endif //_C_MOVEMENT_H
