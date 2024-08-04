#include "game.h"
#include "collision.h"

bool Game_is_entities_full(const struct Game* game)
{
    if (game->entity_count >= ENTITY_MAX-1) {
        return true;
    }
    return false;
}


struct C_Transform* Game_is_colliding(const struct Game* game,
                                      const struct C_Transform* t)
{
    if (t->collision_type == COLLISION_NONE) {
        return NULL;
    }
    
    for (int i = 0; i < ENTITY_MAX; i++)
    {
        if (game->transforms[i].entity_id != -1) {
            const struct C_Transform* t2 = game->transforms + i;
            if (t2->entity_id == t->entity_id) {
                continue;
            }
            
            if (t2->collision_type == COLLISION_NONE) {
                continue;
            }
            
            if ( (t->collision_type == COLLISION_AABB) && (t2->collision_type == COLLISION_AABB) ) {
                //use AABB, its faster
                if (is_colliding_aabb(t, t2)) {
                    return t2;
                }
            }
            else if (is_colliding_sat(t, t2)) {
                return t2;
            }
        }
    }
    
    return NULL;
}


void Game_clear_entity(struct Game* game, int entity_id)
{
    {
        game->transforms[entity_id].entity_id = -1;
        game->movements[entity_id].entity_id = -1;
        game->draws[entity_id].entity_id = -1;
        game->states[entity_id].entity_id = -1;
        game->melees[entity_id].entity_id = -1;
        game->damages[entity_id].entity_id = -1;
    }
}


struct Message* Game_create_message(struct Game* game, int id, int lvalue, int rvalue)
{
    struct Message* msg = game->message_memory + game->message_index;
    if ((game->message_index >= game->message_memory_max) ||
        (msg->id != -1)
        )
        {
        for (int i = 0; i < game->message_memory_max; i++) {
            struct Message* current = game->message_memory + i;

            if (current->id == -1) {
                msg = current;
                break;
            }
        }
    }

    if (msg->id != -1) {
        //message_memory is full
        return NULL;
    }

    msg->id = id;
    msg->lvalue = lvalue;
    msg->rvalue = rvalue;
    msg->_index = game->message_index;

    game->message_index++;

    return msg;
}


void Game_delete_message(struct Game* game, struct Message* msg)
{
    game->message_index = msg->_index;
    msg->id = -1;
}


void Game_send_component_message(struct Game* game,
                                 int entity_id,
                                 const struct Message* msg
                                 )
{
    if (game->states[entity_id].entity_id == entity_id) {
        Queue_push(&(game->states[entity_id].messages),
                   msg);
    }
}
