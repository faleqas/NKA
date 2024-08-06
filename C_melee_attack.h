/* date = July 2nd 2024 10:25 am */

#ifndef _C_MELEE_ATTACK_H
#define _C_MELEE_ATTACK_H
#include "C_transform.h"


struct C_Melee_Attack_Data
{
    struct Transform trans; //transform's x and y values are RELATIVE TO THE C_Transform OF THE ENTITY
    int animation_id;
};

struct C_Melee
{
    int entity_id;
    struct C_Melee_Attack_Data* attacks;
    int attacks_count;
    int current_attack_index;
};


void C_Melee_add_attack(struct C_Melee* attack, int x, int y, int w, int h, int anim_id);

void C_Melee_update(struct C_Melee* attack, struct Game* game);


#endif //_C_MELEE_ATTACK_H
