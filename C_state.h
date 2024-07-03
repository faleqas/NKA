/* date = July 1st 2024 7:04 am */

#ifndef _C_STATE_H
#define _C_STATE_H
#include <stdbool.h>

enum
{
    STATE_NONE,

    STATE_PLAYER_IDLE = 1 << 0,
    STATE_PLAYER_MOVE = 1 << 1,
    STATE_PLAYER_READY_JUMP = 1 << 2,
    STATE_PLAYER_JUMP = 1 << 3,
    STATE_PLAYER_AIR = 1 << 4,
    STATE_PLAYER_DOUBLE_JUMP = 1 << 5, //same as JUMP but with decreased jump force
    STATE_PLAYER_CAN_DOUBLE_JUMP = 1 << 6,

    STATE_ATTACKING_MELEE = 1 << 8, //performing a melee attack
    STATE_ATTACKING_MELEE_DAMAGE = 1 << 9 //should deal the melee attack damage this frame (see C_melee_attack.c)
};

struct C_State
{
    int entity_id;
    int state;
    int state_countdown; //when it is set and then reaches 0, we advance to a next state
                         //used for advancing from READY_JUMP to JUMP for example
    int next_state; //the next state mentioned above

    void (*state_update)(struct C_State*, const struct Game*);
};

//just calls the state_update function pointer if there is one. only wrote for consistency
void C_State_update(struct C_State* c, const struct Game* game);

void player_state_update(struct C_State*, const struct Game*);


#endif //_C_STATE_H
