/* date = July 23rd 2024 5:58 am */

#ifndef _C_DAMAGE_H
#define _C_DAMAGE_H

struct C_Damage
{
    int entity_id;
    int health;
};


void C_Damage_take_damage(struct C_Damage* obj_damage, struct Game* game,
                          int damage);

#endif //_C_DAMAGE_H
