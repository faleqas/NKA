/* date = June 25th 2024 11:09 pm */

#ifndef COMPONENT_H
#define COMPONENT_H
//C_ prefix stands for component
//a component with an id of -1 means its free to replace (it doesn't exist)

#include "C_movement.h"
#include "C_transform.h"
#include "C_draw.h"
#include "C_state.h"
#include "C_melee_attack.h"
#include "C_damage.h"


enum
{
    COMPONENT_NONE,
    COMPONENT_TRANSFORM = 1,
    COMPONENT_MOVEMENT = 1 << 1,
    COMPONENT_DRAW = 1 << 2,
    COMPONENT_STATE = 1 << 3,
    COMPONENT_MELEE_ATTACK = 1 << 4,
    COMPONENT_DAMAGE = 1 << 5
};


//everytime a new component is added
//create_game and Game_clear_entity must be synced


#endif //COMPONENT_H
