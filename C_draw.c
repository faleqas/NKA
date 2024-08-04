#include "C_draw.h"
#include "component.h"
#include "asset_manager.h"
#include "util.h"
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "game.h"
#include "A_animation.h"

//helper functions

//returns true if it was time to advance and we advanced. otherwise false
static bool advance_animation(struct C_Draw* c, const struct A_Animation* anim);

void C_Draw_update(struct C_Draw* c, const struct Game* game)
{
    const struct C_Transform* t = game->transforms + c->entity_id;
    if (t->entity_id != c->entity_id) {
        //we don't actually have a transform to operate on
        //TODO(omar): maybe draw at a default position
        return;
    }
    
    if (c->sprite_update) {
        c->sprite_update(c, game);
    }
    
    struct A_Sprite* sprite = AssetManager_get_sprite(game->asset_m,
                                                      c->sprite_id);
    
    SDL_Rect dst = 
    {
        (int)(t->x - game->camera.x), (int)(t->y - game->camera.y),
        (int)sprite->src.w * c->scale, (int)sprite->src.h * c->scale
    };
    
    //get the flip
    const struct C_State* s = game->states + c->entity_id;
    if (s->entity_id != c->entity_id) {
        //use flip assigned at creation
    }
    else {
        if (s->dir_x == 1) {
            c->flip = SDL_FLIP_NONE;
        }
        else {
            c->flip = SDL_FLIP_HORIZONTAL;
            dst.x -= (sprite->src.w * c->scale) - t->w;
            //dst.x -= dst.w / 2;
        }
    }
    
    SDL_RenderCopyEx(game->renderer, sprite->texture,
                     &(sprite->src), &dst,
                     0, NULL,
                     c->flip);
    
    SDL_Rect r = {
        (int)(t->x - game->camera.x), (int)(t->y - game->camera.y),
        (int)t->w, (int)t->h
    };
    
    if (game->draw_collisions) {
        SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(game->renderer, &r);
    }
    
    c->tics_since_frame_change++;
}


void player_sprite_update(struct C_Draw* c, const struct Game* game)
{
    //we don't have to check if the transform is ours since that is done in C_Draw_update
    const struct C_Transform* trans = game->transforms + c->entity_id;
    const struct C_Movement* move = game->movements + c->entity_id;
    const struct C_State* state = game->states + c->entity_id;
    if (state->entity_id != c->entity_id) {
        //player must have a state component
        util_assert(false, "Player must have a C_State component\n");
        return;
    }
    if (move->entity_id != c->entity_id) {
        util_assert(false, "Why does the player not have a movement component ?");
        return;
    }
    
    int anim = 0;
    //printf("%f\n", move->velocity_y);
    //Actual logic
    if (move->on_wall) {
        anim = ANIM_PLAYER_WALL_SLIDE;
    }
    else if (state->state & STATE_ATTACKING_MELEE) {
        anim = ANIM_PLAYER_ATTACK_0;
    }
    else if (state->state & STATE_PLAYER_AIR) {
        if (move->velocity_y < -(PLAYER_JUMP_FORCE * 0.3f)) {
            anim = ANIM_PLAYER_FIRST_JUMP;
        }
        else if (move->velocity_y > (GRAVITY * 65)) {
            anim = ANIM_PLAYER_FALLING;
        }
        else {
            anim = ANIM_PLAYER_AIR_JUMP;
        }
    }
    else if (state->state & STATE_PLAYER_READY_JUMP) {
        anim = ANIM_PLAYER_READY_JUMP;
    }
    else if (state->state & STATE_PLAYER_IDLE) {
        //IDLE
        anim = ANIM_PLAYER_IDLE_0;
    }
    else if (state->state & STATE_PLAYER_MOVE) {
        anim = ANIM_PLAYER_RUN;
    }
    
    if (c->prev_anim_id != anim) {
        C_Draw_reset_animation(c, game);
    }
    
    struct A_Animation* anim_p = AssetManager_get_animation(game->asset_m, anim);
    
    if (state->state & STATE_ATTACKING_MELEE) {
        if (c->anim_index == 6) {
            if (c->tics_since_frame_change == 1) {
                //printf("%d\n", c->tics_since_frame_change);
                //Queue_push(&(state->messages), MESSAGE_MATTACK_DAMAGE_FRAME);
                struct Message* msg = Game_create_message(game, MESSAGE_MATTACK_DAMAGE_FRAME, 0, 0);
                Game_send_component_message(game, c->entity_id,
                                            msg);
            }
        }
    }
    
    if (advance_animation(c, anim_p)) {
        //Game_send_component_message(game, c->entity_id,
               //MESSAGE_FRAME_ADVANCE);
    }
    
    c->prev_anim_id = anim;
}


void C_Draw_reset_animation(struct C_Draw* c, const struct Game* game)
{
    c->anim_index = 0;
}


static bool advance_animation(struct C_Draw* c, const struct A_Animation* anim)
{
    if (anim == NULL) {
        return false;
    }
    
    if (c->anim_index >= anim->len) {
        if (anim->loop) {
            c->anim_index = 0;
        }
        else {
            c->anim_index = anim->len-1;
        }
    }
    
    c->sprite_id = anim->sprites[c->anim_index];
    int duration = anim->durations[c->anim_index];
    int elapsed = SDL_GetTicks() - c->last_change_tic;
    
    if (elapsed >= duration) {
        c->anim_index++;
        c->last_change_tic = SDL_GetTicks();
        c->tics_since_frame_change = 0;
        return true;
    }
    
    return false;
}