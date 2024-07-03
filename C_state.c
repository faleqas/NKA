#include "C_state.h"
#include "component.h"
#include "game.h"
#include <stdio.h>

void C_State_update(struct C_State* c, const struct Game* game)
{
    if (c->state_update)
    {
        c->state_update(c, game);
    }
}

void player_state_start_attack(struct C_State* c, const struct Game* game)
{
    c->state_countdown = 120;
    c->next_state = (c->state & ~STATE_ATTACKING_MELEE) & ~STATE_ATTACKING_MELEE_DAMAGE;
}

void player_state_update(struct C_State* c, const struct Game* game)
{
    //TODO(omar): this should be moved somewhere else. maybe not part of c->state but c->flags or something
    static bool space_released_in_jump = false;


    const uint8_t* keys = SDL_GetKeyboardState(NULL);

    struct C_Movement* m = game->movements + c->entity_id;
    if (m->entity_id != c->entity_id) {
        return;
    }

    if (m->on_ground) {
        c->state |= STATE_PLAYER_CAN_DOUBLE_JUMP;
        space_released_in_jump = false;
    }
    
    if (keys[SDL_SCANCODE_D]) {
        c->state |= STATE_PLAYER_MOVE;
        c->state &= ~STATE_PLAYER_IDLE;
        m->dir_x = 1;
    }
    else if (keys[SDL_SCANCODE_A]) {
        c->state |= STATE_PLAYER_MOVE;
        c->state &= ~STATE_PLAYER_IDLE;
        m->dir_x = -1;
    }
    else {
        c->state &= ~STATE_PLAYER_MOVE;
        c->state |= STATE_PLAYER_IDLE;
    }

    if (keys[SDL_SCANCODE_SPACE]) {
        if (m->on_ground) {
            if (!(c->state & STATE_PLAYER_READY_JUMP)) {
                c->state |= STATE_PLAYER_READY_JUMP;
                c->state_countdown = 10;
            }
        }
        else if (c->state & STATE_PLAYER_CAN_DOUBLE_JUMP) {
            if (m->velocity_y > (-PLAYER_JUMP_FORCE * 0.3f)) {
                if (space_released_in_jump) {
                    c->state |= STATE_PLAYER_DOUBLE_JUMP;
                    c->state &= ~STATE_PLAYER_CAN_DOUBLE_JUMP;
                }
            }
        }
        else {
            c->state &= ~STATE_PLAYER_JUMP;
            c->state &= ~STATE_PLAYER_DOUBLE_JUMP;
        }
    }
    if (m->velocity_y) {
        c->state &= ~STATE_PLAYER_JUMP;
        c->state |= STATE_PLAYER_AIR;
        if (!(keys[SDL_SCANCODE_SPACE])) {
            space_released_in_jump = true;
        }
    }
    else {
        c->state &= ~STATE_PLAYER_AIR;
    }

    if (c->state_countdown) {
        c->state_countdown--;
    }
    else {
        if (c->state & STATE_PLAYER_READY_JUMP) {
            c->state &= ~STATE_PLAYER_READY_JUMP;
            c->state |= STATE_PLAYER_JUMP;
        }
        else if (c->next_state) {
            c->state = c->next_state;
            c->next_state = STATE_NONE;
        }
    }

    if (game->keys_just_released[SDL_SCANCODE_RIGHT]) {
        if (!(c->state & STATE_ATTACKING_MELEE)) {
            m->dir_x = 1;
            c->state |= STATE_ATTACKING_MELEE_DAMAGE;
            c->state |= STATE_ATTACKING_MELEE;
        }
    }
    else if (game->keys_just_released[SDL_SCANCODE_LEFT]) {
        if (!(c->state & STATE_ATTACKING_MELEE)) {
            m->dir_x = -1;
            c->state |= STATE_ATTACKING_MELEE_DAMAGE;
            c->state |= STATE_ATTACKING_MELEE;
        }
    }

    if (c->state & STATE_ATTACKING_MELEE_DAMAGE) {
        player_state_start_attack(c, game);
    }
}