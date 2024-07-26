/* date = June 25th 2024 11:00 pm */

#ifndef _C_SPRITE_H
#define _C_SPRITE_H

#include <SDL2/SDL.h>
#include "game.h"


struct C_Sprite
{
    int entity_id;
    SDL_Rect src; //by default its set to the entire image
};


void C_Sprite_init(struct C_Sprite* c, const struct Game* game
                   );
void C_Sprite_draw(struct C_Sprite* c, const struct Game* game);


#endif //_C_SPRITE_H
