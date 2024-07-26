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


void Game_send_component_message(struct Game* game,
                                 int entity_id,
                                 const int msg
                                 )
{
    if (game->states[entity_id].entity_id == entity_id) {
        Queue_push(&(game->states[entity_id].messages),
                   msg);
    }
}
