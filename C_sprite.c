#include "C_sprite.h"
#include "game.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <util.h>
#include <stdio.h>
#include "C_transform.h"


void C_Sprite_init(struct C_Sprite* c, const struct Game* game)
{
    /*
    c->_texture = IMG_LoadTexture(game->renderer, path);
    util_assert(c->_texture != NULL, "C_Sprite: Loading SDL_Texture failed\n");
    
    int texture_w;
    int texture_h;
    SDL_QueryTexture(c->_texture, NULL, NULL, &texture_w, &texture_h);
    c->src.w = texture_w;
    c->src.h = texture_h;
*/
}


void C_Sprite_draw(struct C_Sprite* c, const struct Game* game)
{
    /*
    if (c->_texture == NULL) {
        return;
    }
    struct C_Transform* t = game->transforms + c->entity_id;
    if (t->entity_id != c->entity_id) {
        //we don't actually have a transform to operate on
        //TODO(omar): maybe draw at a default position
        return;
    }
    SDL_Rect dst = 
    {
        (int)t->x, (int)t->y,
        t->w, t->h
    };
    SDL_RenderCopy(game->renderer, c->_texture,
                   &(c->src), &dst);
*/
}

