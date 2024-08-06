#include "entity.h"
#include "game.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <util.h>
#include <stdio.h>
#include "component.h"
#include "util.h"
#include "asset_manager.h"
#include "queue.h"


void E_Dummy_create(struct Game* game, float x, float y)
{
    util_assert(Game_is_entities_full(game) == false, "Entity creation failed: ENTITY_MAX has been reached.\n");
    
    int id = game->entity_count;
    
    struct A_Sprite* sprite = AssetManager_get_sprite(game->asset_m,
                                                      SPRITE_PLAYER_IDLE_00);
    {
        struct C_Transform* t = game->transforms + id;
        t->x = x;
        t->y = y;
        t->w = sprite->src.w * 3;
        t->h = sprite->src.h * 3;
        t->entity_id = id;
        t->collision_type = COLLISION_AABB;
    }
    {
        struct C_Draw* d = game->draws + id;
        d->entity_id = id;
        d->sprite_id = SPRITE_PLAYER_IDLE_00;
        d->flip = SDL_FLIP_NONE;
        d->sprite_update = NULL;
        d->anim_index = 0;
        d->last_change_tic = 0;
        d->scale = 3.0f;
    }
    {
        struct C_Movement* m = game->movements + id;
        m->velocity_x = 0.2f;
        m->dir_x = 1;
        m->entity_id = id;
        m->type = MOVETYPE_DUMMY;
    }
    {
        struct C_Damage* d = game->damages + id;
        d->entity_id = id;
        d->health = 1;
    }
    {
        struct C_State* s = game->states + id;
        s->entity_id = id;
        s->state = STATE_NONE;
        s->state_update = &dummy_state_update;
        s->state_countdown = 0;
        s->next_state = STATE_NONE;
        Queue_init(&(s->messages), 0);
    }
    
    game->entity_count++;
}

int E_Player_create(struct Game* game, float x, float y)
{
    util_assert(Game_is_entities_full(game) == false, "Entity creation failed: ENTITY_MAX has been reached.\n");
    
    int id = game->entity_count;
    
    struct A_Sprite* idle_sprite = AssetManager_get_sprite(game->asset_m,
                                                           SPRITE_PLAYER_IDLE_00);
    
    struct C_Transform* t = game->transforms + id;
    t->x = x;
    t->y = y;
    t->w = idle_sprite->src.w * 3;
    t->h = idle_sprite->src.h * 3;
    t->entity_id = id;
    t->collision_type = COLLISION_AABB;
    {
        struct C_Movement* m = game->movements + id;
        m->max_speed = 1.0f;
        m->accel = 0.017f;
        m->decel = 0.04f;
        m->velocity_x = 0;
        m->velocity_y = 0;
        m->on_ground = false;
        m->dir_x = 1;
        m->entity_id = id;
        m->type = MOVETYPE_PLAYER;
    }
    {
        struct C_Draw* d = game->draws + id;
        d->entity_id = id;
        d->sprite_id = SPRITE_PLAYER_IDLE_00;
        d->flip = SDL_FLIP_NONE;
        d->sprite_update = &player_sprite_update;
        d->anim_index = 0;
        d->last_change_tic = 0;
        d->scale = 3.0f;
    }
    {
        struct C_State* s = game->states + id;
        s->entity_id = id;
        s->state = STATE_PLAYER_MOVE;
        s->state_update = &player_state_update;
        s->state_countdown = 0;
        s->next_state = STATE_NONE;
        s->dir_x = 1;
        Queue_init(&(s->messages), 0);
        
        C_State_add_state(s, STATE_PLAYER_MOVE_WITH_INPUT);
    }
    {
        struct C_Melee* mattack = game->melees + id;
        mattack->entity_id = id;
        mattack->current_attack_index = 0;

        C_Melee_add_attack(mattack, t->w, 0, 46, t->h, ANIM_PLAYER_ATTACK_0);
        C_Melee_add_attack(mattack, t->w, 0, 64, t->h, ANIM_PLAYER_ATTACK_1);
    }
    game->entity_count++;
    
    return id;
}


void E_Block_create(struct Game* game, float x, float y)
{
    util_assert(Game_is_entities_full(game) == false, "Entity creation failed: ENTITY_MAX has been reached.\n");
    
    int id = game->entity_count;
    
    struct A_Sprite* sprite = AssetManager_get_sprite(game->asset_m,
                                                      SPRITE_DIRT);
    
    struct C_Transform* t = game->transforms + id;
    t->x = x;
    t->y = y;
    t->w = TILE_SIZE;
    t->h = TILE_SIZE;
    t->entity_id = id;
    t->collision_type = COLLISION_AABB;
    
    struct C_Draw* d = game->draws + id;
    d->entity_id = id;
    d->sprite_id = SPRITE_DIRT;
    d->flip = SDL_FLIP_NONE;
    d->sprite_update = NULL;
    d->anim_index = 0;
    d->last_change_tic = 0;
    d->scale = 2.0f;
    
    game->entity_count++;
}
