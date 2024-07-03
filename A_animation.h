/* date = June 29th 2024 4:14 am */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdbool.h>

//again a hardcoded constant limit on size. dynamic size allocation is not yet needed
#define MAX_ANIMATION_SIZE 100

enum
{
    ANIM_NONE,
    ANIM_PLAYER_IDLE_0, //sword tucked idle
    ANIM_PLAYER_RUN,
    ANIM_PLAYER_READY_JUMP, //crouching before jumping
    ANIM_PLAYER_FIRST_JUMP, //in the air first stage (hands up)
    ANIM_PLAYER_AIR_JUMP, //in the air second stage (ball)
    ANIM_PLAYER_FALLING,
    ANIM_PLAYER_ATTACK_0
};

struct A_Animation
{
    int id;
    unsigned int len; //amount of sprites in the animation
    bool loop;
    int sprites[MAX_ANIMATION_SIZE]; //actual sprite ids
    unsigned int durations[MAX_ANIMATION_SIZE]; //how long each sprite should last in milliseconds
};

#endif //ANIMATION_H
