#include "C_movement.h"
#include "C_transform.h"
#include "game.h"
#include <util.h>
#include <stdio.h>

static void C_Movement_halt_to_stop(struct C_Movement* c, struct Game* game, float decel_factor);
//static void player_movement_update(struct C_Movement* c, struct Game* game);

void C_Movement_update(struct C_Movement* c, struct Game* game)
{
    struct C_Transform* t = game->transforms + c->entity_id;
    if (t->entity_id != c->entity_id) {
        //we don't actually have a transform to operate on
        return;
    }
    
    switch (c->type)
    {
        case MOVETYPE_PLAYER:
        {
            struct C_State* state = game->states + c->entity_id;
            if (state->entity_id != c->entity_id) {
                //player must have a state component
                util_assert(false, "Player must have a C_State component\n");
                return;
            }
            
            if (!(c->on_ground)) {
                c->velocity_y += GRAVITY;
                if (c->velocity_y > MAX_GRAVITY) {
                    c->velocity_y = MAX_GRAVITY;
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
                    printf("%f\n", c->velocity_x);
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
            }
            
            t->y += c->velocity_y;
            if (Game_is_colliding(game, t)) {
                t->y -= c->velocity_y;
                if (c->velocity_y > 0) {
                    c->on_ground = true;
                }
                else {
                    c->velocity_y = 0;
                }
            }

            t->y += TILE_SIZE;
            if (!(Game_is_colliding(game, t))) {
                c->on_ground = false;
            }
            t->y -= TILE_SIZE;
            
        } break;
        
        default: break;
    }
}


static void C_Movement_halt_to_stop(struct C_Movement* c, struct Game* game, float decel_factor)
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