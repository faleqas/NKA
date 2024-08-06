#include "C_melee_attack.h"
#include "game.h"
#include <util.h>
#include <stdio.h>
#include "asset_manager.h"

#include <malloc.h>
#include <memory.h>



void C_Melee_add_attack(struct C_Melee* attack, int x, int y, int w, int h, int anim_id)
{
    attack->attacks_count++;

    if (attack->attacks == NULL) {
        attack->attacks = malloc(sizeof(struct C_Melee_Attack_Data) * attack->attacks_count);
    }
    else {
        attack->attacks = realloc(attack->attacks, sizeof(struct C_Melee_Attack_Data) * attack->attacks_count);
    }

    struct C_Melee_Attack_Data* data = attack->attacks + (attack->attacks_count-1);

    data->trans.x = x;
    data->trans.y = y;
    data->trans.w = w;
    data->trans.h = h;
    data->animation_id = anim_id;
}


void C_Melee_update(struct C_Melee* attack, struct Game* game)
{
    util_assert(attack, "");
    
    const struct C_Transform* entity_trans = game->transforms + attack->entity_id;
    if (entity_trans->entity_id != attack->entity_id) {
        return;
    }
    
    struct C_State* state = game->states + attack->entity_id;
    if (state->entity_id != attack->entity_id) {
        return;
    }
    
    const struct C_Draw* draw = game->draws + attack->entity_id;
    if (draw->entity_id != attack->entity_id) {
        return;
    }
    
    
    struct C_Transform trans; //its a C_Transform because thats what game_is_colliding takes.

    struct C_Melee_Attack_Data* attack_data = attack->attacks + attack->current_attack_index;
    
    trans.entity_id = attack->entity_id; //to make sure we don't attack ourselves
    trans.collision_type = COLLISION_AABB;
    trans.x = entity_trans->x + attack_data->trans.x;
    trans.y = entity_trans->y + attack_data->trans.y;
    trans.w = attack_data->trans.w;
    trans.h = attack_data->trans.h;
    
    if (draw->flip == SDL_FLIP_HORIZONTAL) {
        trans.x -= (attack_data->trans.w * 2) + (entity_trans->w - attack_data->trans.w);
    }
    
    SDL_Rect r = {
        (int)(trans.x - game->camera.x), (int)(trans.y - game->camera.y),
        (int)trans.w, (int)trans.h
    };
    
    if (game->draw_collisions) {
        SDL_SetRenderDrawColor(game->renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(game->renderer, &r);
    }
    
    
    if (state->state & STATE_ATTACKING_MELEE_DAMAGE) {
        
        struct C_Transform* collider_trans = Game_is_colliding(game, &trans);
        
        if (collider_trans) {
            struct C_Damage* collider_dmg = game->damages + collider_trans->entity_id;
            
            if (collider_dmg) {
                C_Damage_take_damage(collider_dmg, game, 1);
            }
        }
        
        state->state &= ~STATE_ATTACKING_MELEE_DAMAGE;
    }
}
