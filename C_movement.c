#include "C_movement.h"
#include "C_transform.h"
#include "game.h"
#include <util.h>
#include <stdio.h>
#include <math.h>

void C_Movement_update(struct C_Movement* c, struct Game* game)
{
    struct C_Transform* t = game->transforms + c->entity_id;
    if (t->entity_id != c->entity_id) {
        //we don't actually have a transform to operate on
        return;
    }
    
    c->colliding_x = false;
    c->colliding_y = false;
    
    switch (c->type)
    {
        case MOVETYPE_DUMMY:
        {
            struct C_Transform* player_trans = game->transforms + game->e_player_id;
            struct C_Movement* player_move = game->movements + game->e_player_id;
            int player_should_face = 1;
            
            if (t->x < player_trans->x) {
                player_should_face = -1;
            }
            
            if (player_move->dir_x != player_should_face) {
                C_Movement_face_target(c, t, player_trans, 0);
                t->x += c->velocity_x * c->dir_x;
                if (Game_is_colliding(game, t)) {
                    t->x -= c->velocity_x * c->dir_x;
                }
            }
            
        } break;
        case MOVETYPE_PLAYER:
        {
            struct C_State* state = game->states + c->entity_id;
            if (state->entity_id != c->entity_id) {
                //player must have a state component
                util_assert(false, "Player must have a C_State component\n");
                return;
            }
            
            if (!(c->on_ground)) {
                if (!(c->on_wall)) {
                    c->velocity_y += GRAVITY;
                    if (c->velocity_y > MAX_GRAVITY) {
                        c->velocity_y = MAX_GRAVITY;
                    }
                }
            }
            else {
                c->velocity_y = 0;
            }
            
            if (state->state & STATE_PLAYER_JUMP) {
                c->velocity_y = -PLAYER_JUMP_FORCE;
                c->on_ground = false;
            }
            else if (state->state & STATE_PLAYER_DOUBLE_JUMP) {
                c->velocity_y = -PLAYER_JUMP_FORCE;
                c->on_ground = false;
            }
            
            if (state->state & STATE_ATTACKING_MELEE) {
                if (state->dir_x != c->dir_x) {
                    C_Movement_halt_to_stop(c, game, 1.0f);
                }
                else if (state->state & STATE_PLAYER_AIR) {
                    C_Movement_halt_to_stop(c, game, 0.06f); //slow down a bit in the air but don't completely stop moving
                    state->state &= ~STATE_PLAYER_MOVE;
                }
            }
            if (state->state & STATE_PLAYER_MOVE) {
                if (c->dir_x == 1) {
                    c->velocity_x += c->accel * c->dir_x;
                    
                    if (c->velocity_x > c->max_speed) {
                        c->velocity_x = c->max_speed;
                    }
                }
                else if (c->dir_x == -1) {
                    c->velocity_x -= c->accel;
                    
                    if (c->velocity_x < -(c->max_speed)) {
                        c->velocity_x = -(c->max_speed);
                    }
                }
            }
            else if (state->state & STATE_PLAYER_IDLE) {
                C_Movement_halt_to_stop(c, game, 1);
            }
            
            t->x += c->velocity_x;
            if (Game_is_colliding(game, t)) {
                t->x -= c->velocity_x;
                c->colliding_x = true;
            }
            
            t->y += c->velocity_y;
            if (Game_is_colliding(game, t)) {
                t->y -= c->velocity_y;
                c->colliding_y = true;
                if (c->velocity_y > 0) {
                    c->on_ground = true;
                }
                else {
                    c->velocity_y = 0;
                }
            }
            
            if ((c->colliding_x) && !(c->on_ground)) {
                c->on_wall = true;
            }
            else {
                c->on_wall = false;
            }
            
            //This is for cases where the player didn't jump but walked off a ledge
            t->y += TILE_SIZE;
            if (!(Game_is_colliding(game, t))) {
                c->on_ground = false;
            }
            t->y -= TILE_SIZE;
            
            
        } break;
        
        default: break;
    }
}


void C_Movement_halt_to_stop(struct C_Movement* c, struct Game* game, float decel_factor)
{
    if (c->velocity_x > 0) {
        c->velocity_x -= c->decel * decel_factor;
        if (c->velocity_x < 0) {
            c->velocity_x = 0;
        }
    }
    else if (c->velocity_x < 0) {
        c->velocity_x += c->decel * decel_factor;
        if (c->velocity_x > 0) {
            c->velocity_x = 0;
        }
    }
}


void C_Movement_flip_dir(struct C_Movement* c)
{
    c->velocity_x = -c->velocity_x;
    c->dir_x = -c->dir_x;
}


void C_Movement_face_target(struct C_Movement* c,
                            const struct C_Transform* trans,
                            const struct C_Transform* target,
                            int dead_zone)
{
    if (dead_zone) {
        int dist = abs(trans->x - target->x);
        if (dist < dead_zone) {
            c->dir_x = 0;
            return;
        }
    }
    if (target->x > trans->x) {
        c->dir_x = 1;
    }
    else if (target->x < trans->x) {
        c->dir_x = -1;
    }
    else {
        c->dir_x = 0;
    }
}










