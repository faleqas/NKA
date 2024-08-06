#include "C_state.h"
#include "component.h"
#include "game.h"
#include "C_movement.h"
#include <stdio.h>
#include "A_animation.h"

void C_State_update(struct C_State* c, const struct Game* game)
{
    if (c->state_update)
    {
        c->state_update(c, game);
    }
}


void C_State_add_state(struct C_State* c, const int state)
{
    c->state |= state;
}


void C_State_remove_state(struct C_State* c, const int state)
{
    c->state &= ~state;
}


void C_State_set_state(struct C_State* c, const int state)
{
    c->state = state;
}


void C_State_go_to_next_state(struct C_State* c)
{
    if (!(c->next_state)) {
        return;
    }
    
    c->state = c->next_state;
    c->next_state = STATE_NONE;
}


void player_state_start_attack(struct C_State* c, const struct Game* game)
{
    c->state_countdown = 120;
    c->next_state = (c->state & ~STATE_ATTACKING_MELEE) & ~STATE_ATTACKING_MELEE_DAMAGE;
}

void player_state_update(struct C_State* c, const struct Game* game)
{
    struct Message* msg = Queue_pop(&(c->messages));
    if (msg) {
        switch (msg->id)
        {
            case MESSAGE_MATTACK_DAMAGE_FRAME:
            {
                C_State_add_state(c, STATE_ATTACKING_MELEE_DAMAGE);
            } break;
            
            case MESSAGE_FRAME_ADVANCE:
            {
                
            } break;
            
            case MESSAGE_DIE:
            {
                Game_clear_entity(game, c->entity_id);
            } break;
            
            default:
            {
                
            } break;
        }
        Game_delete_message(game, msg);
    }
    
    //TODO(omar): this should be moved somewhere else. maybe not part of c->state but c->flags or something
    static bool space_released_in_jump = false;
    
    struct C_Movement* m = game->movements + c->entity_id;
    if (m->entity_id != c->entity_id) {
        return;
    }
    
    struct C_Draw* d = game->movements + c->entity_id;
    if (d->entity_id != c->entity_id) {
        return;
    }

    struct C_Melee* melee = game->melees + c->entity_id;
    if (melee->entity_id != c->entity_id) {
        return;
    }
    
    C_State_remove_state(c, STATE_PLAYER_DOUBLE_JUMP);
    
    if (m->on_ground) {
        C_State_add_state(c, STATE_PLAYER_CAN_DOUBLE_JUMP);
        C_State_add_state(c, STATE_PLAYER_MOVE_WITH_INPUT);
        space_released_in_jump = false;
    }
    
    if (m->on_wall) {
        m->velocity_y = 0;
        C_State_remove_state(c, STATE_PLAYER_CAN_DOUBLE_JUMP);
        C_State_add_state(c, STATE_PLAYER_MOVE_WITH_INPUT);
    }
    else if (m->velocity_y) {
        C_State_remove_state(c, STATE_PLAYER_JUMP);
        C_State_add_state(c, STATE_PLAYER_AIR);
        
        if (!(game->keys_held[SDL_SCANCODE_SPACE])) {
            space_released_in_jump = true;
        }
    }
    else {
        C_State_remove_state(c, STATE_PLAYER_AIR);
    }
    
    if (c->state & STATE_PLAYER_MOVE_WITH_INPUT) {
        if (game->keys_held[SDL_SCANCODE_D]) {
            C_State_add_state(c, STATE_PLAYER_MOVE);
            C_State_remove_state(c, STATE_PLAYER_IDLE);
            
            m->dir_x = 1;
            
            if (!(c->state & STATE_ATTACKING_MELEE)) {
                //can't change attack dir by moving
                c->dir_x = 1;
            }
            
            if (m->on_wall) {
                m->velocity_x = 0;
            }
            
        }
        else if (game->keys_held[SDL_SCANCODE_A]) {
            C_State_add_state(c, STATE_PLAYER_MOVE);
            C_State_remove_state(c, STATE_PLAYER_IDLE);
            
            m->dir_x = -1;
            
            if (!(c->state & STATE_ATTACKING_MELEE)) {
                //can't change attack dir by moving
                c->dir_x = -1;
            }
            
            if (m->on_wall) {
                m->velocity_x = 0;
            }
        }
        else {
            C_State_remove_state(c, STATE_PLAYER_MOVE);
            C_State_add_state(c, STATE_PLAYER_IDLE);
        }
    }
    
    if (game->keys_just_pressed[SDL_SCANCODE_SPACE]) {
        if (m->on_ground) {
            if (!(c->state & STATE_PLAYER_READY_JUMP)) {
                C_State_add_state(c, STATE_PLAYER_READY_JUMP);
                c->state_countdown = 10;
            }
        }
        
        else if (c->state & STATE_PLAYER_CAN_DOUBLE_JUMP) {
            if (m->velocity_y > (-PLAYER_JUMP_FORCE * 0.3f)) {
                if (space_released_in_jump) {
                    C_State_add_state(c, STATE_PLAYER_DOUBLE_JUMP);
                    C_State_remove_state(c, STATE_PLAYER_CAN_DOUBLE_JUMP);
                }
            }
        }
        
        else if (m->on_wall) {
            m->velocity_x = 2.0f * -(m->last_collision_x);
            m->velocity_y = -PLAYER_JUMP_FORCE;
            m->dir_x = -(m->last_collision_x);
            c->next_state = c->state | STATE_PLAYER_CAN_DOUBLE_JUMP;
            C_State_remove_state(c, STATE_PLAYER_MOVE_WITH_INPUT);
            C_State_remove_state(c, STATE_PLAYER_IDLE);
            C_State_remove_state(c, STATE_PLAYER_MOVE);
            
            c->state_countdown = 60;
            
            c->dir_x = m->dir_x;
        }
        
        else {
            C_State_remove_state(c, STATE_PLAYER_JUMP);
            C_State_remove_state(c, STATE_PLAYER_DOUBLE_JUMP);
        }
    }
    
    if (c->state_countdown) {
        c->state_countdown--;
    }
    else {
        if (c->state & STATE_ATTACKING_MELEE) {
            C_Melee_advance_combo(melee);
        }
        if (c->state & STATE_PLAYER_READY_JUMP) {
            C_State_remove_state(c, STATE_PLAYER_READY_JUMP);
            C_State_add_state(c, STATE_PLAYER_JUMP);
        }
        else if (c->next_state) {
            C_State_go_to_next_state(c);
        }
    }
    
    if (game->keys_just_pressed[SDL_SCANCODE_RIGHT]) {
        if (!(c->state & STATE_ATTACKING_MELEE)) {
            c->dir_x = 1;
            //C_State_add_state(c, STATE_ATTACKING_MELEE_DAMAGE);
            C_State_add_state(c, STATE_ATTACKING_MELEE);
            player_state_start_attack(c, game);
        }
    }
    else if (game->keys_just_pressed[SDL_SCANCODE_LEFT]) {
        if (!(c->state & STATE_ATTACKING_MELEE)) {
            c->dir_x = -1;
            //C_State_add_state(c, STATE_ATTACKING_MELEE_DAMAGE);
            C_State_add_state(c, STATE_ATTACKING_MELEE);
            player_state_start_attack(c, game);
        }
    }
}

void dummy_state_update(struct C_State* c, const struct Game* game)
{
    struct Message* msg = Queue_pop(&(c->messages));
    if (msg) {
        switch (msg->id)
        {
            case MESSAGE_DIE:
            {
                Game_clear_entity(game, c->entity_id);
            } break;
            
            default:
            {
                
            } break;
        }
    }
}
