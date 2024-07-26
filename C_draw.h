/* date = June 27th 2024 11:41 am */

#ifndef _C__DRAW_H
#define _C__DRAW_H
#include <stdbool.h>
#include <SDL2/SDL.h>

struct C_Draw
{
    int entity_id;
    int sprite_id; //id of the current sprite to be drawn
    SDL_RendererFlip flip;
    
    //animation (used by sprite_update)
    int anim_index; //index of current sprite in the animation (if there is one)
    int last_change_tic; //the tic on which the last animation frame change occurred
    int prev_anim_id;
    
    int tics_since_frame_change; //tics since frame change duh
    
    float scale;
    
    //function that determines the sprite to be drawn each frame, varies between entities.
    //if an entity only uses 1 sprite then this is NULL
    void (*sprite_update)(struct C_Draw*, const struct Game*);
};

void C_Draw_update(struct C_Draw* c, const struct Game* game);
void C_Draw_reset_animation(struct C_Draw* c, const struct Game* game);

void player_sprite_update(struct C_Draw* c, const struct Game* game);

#endif //_C__DRAW_H
