/* date = July 2nd 2024 10:25 am */

#ifndef _C_MELEE_ATTACK_H
#define _C_MELEE_ATTACK_H
#include "C_transform.h"

/*defines an entity's melee attack's properties*/

struct C_Melee
{
    int entity_id;
    struct Transform trans; //transform's x and y values are RELATIVE TO THE C_Transform OF THE ENTITY
};


void C_Melee_update(struct C_Melee* attack, struct Game* game);


#endif //_C_MELEE_ATTACK_H
