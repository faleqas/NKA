#include "asset_manager.h"
#include <SDL2/SDL_image.h>
#include "game.h"
#include "util.h"
#include <stdio.h>
#include <memory.h>

//used internally only
static int AssetManager_load_sprite_from_texture(struct AssetManager* m, const struct Game* game,
                             int id, SDL_Texture* texture, const SDL_Rect* src);

struct AssetManager* AssetManager_create()
{
    struct AssetManager* m = malloc(sizeof(struct AssetManager));
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        m->sprites[i].id = -1;
        m->sprites[i].texture = NULL;
    }
    for (int i = 0; i < MAX_ANIMATIONS; i++)
    {
        m->animations[i].id = -1;
        m->animations[i].len = 0;
    }
    
    return m;
}


void AssetManager_destroy(struct AssetManager* m)
{
    util_assert(m != NULL, "AssetManager_destroy: Trying to destroy NULL ? what the fuck\n");
    
    free(m);
}


struct A_Sprite* AssetManager_get_sprite(const struct AssetManager* m, int id)
{
    if (id == -1) {
        return NULL;
    }
    
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (m->sprites[i].id == id) {
            return m->sprites + i;
        }
    }
    
    return NULL;
}


struct A_Animation* AssetManager_get_animation(const struct AssetManager* m, int id)
{
    if (id == -1) {
        return NULL;
    }
    
    for (int i = 0; i < MAX_ANIMATIONS; i++)
    {
        if (m->animations[i].id == id) {
            return m->animations + i;
        }
    }
    
    return NULL;
}


static int AssetManager_load_sprite_from_texture(struct AssetManager* m, const struct Game* game,
                             int id, SDL_Texture* texture, const SDL_Rect* src)
{
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (m->sprites[i].id == -1) {
            //its free
            m->sprites[i].id = id;
            m->sprites[i].texture = texture;

            if (src) {
                SDL_Rect* dst_src = &(m->sprites[i].src);
                memcpy(dst_src, src, sizeof(SDL_Rect));
            }
            else {      
                m->sprites[i].src.x = 0;
                m->sprites[i].src.y = 0;
                
                int* w = &(m->sprites[i].src.w);
                int* h = &(m->sprites[i].src.h);
                SDL_QueryTexture(texture, NULL, NULL,
                                w, h);
            }
            return 0;
        }
    }

    return -1;
}


int AssetManager_load_sprite(struct AssetManager* m, const struct Game* game, const char* path,
                             int id, const SDL_Rect* src)
{
    SDL_Texture* texture = IMG_LoadTexture(game->renderer, path);
    util_assert(texture != NULL, "A_Sprite: Loading sprite failed. Path is probably invalid\n");
    
    return AssetManager_load_sprite_from_texture(m, game, id, texture, src);
}

//TODO(omar): a great optimization would be loading all needed tilemaps into memory and then making their sprites
//be just a SDL_Rect that specifies the part of the tilemap they are in AND the id of that tilemap.
//would save us memory and loading time
//we will need to have multiple types of sprites though (a tilemap one and a normal one) and it will be
//a bit of a headache
//we will only do it if we must sacrifice the consistency for performance reasons

//TODO(omar): add bound checking
int AssetManager_load_sprite_from_tilemap(struct AssetManager* m, const struct Game* game, const char* path,
                             int id, unsigned int tile_x, unsigned int tile_y,
                             unsigned int tile_count_x, unsigned tile_count_y,
                             const SDL_Rect* src)
{
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        return -2;
    }

    int tile_width = surf->w / tile_count_x;
    int tile_height = surf->h / tile_count_y;
    int x = tile_x * tile_width;
    int y = tile_y * tile_height;
    printf("%d, %d\n", tile_width, tile_height);

    SDL_Surface* tile_surf = SDL_CreateRGBSurfaceWithFormat(0, tile_width, tile_height, surf->pitch, 
    surf->format->format);
    
    SDL_Rect src_r = {
        x, y,
        tile_width, tile_height
    };

    SDL_Rect dst_r = { //since im not sure if NULL for dstrect is accepted
        0, 0,
        0, 0 //w and h are ignored
    };

    int code = SDL_BlitSurface(surf, &src_r, tile_surf, &dst_r);

    if (code == 0) {
        SDL_Texture* tile_tex = SDL_CreateTextureFromSurface(game->renderer, tile_surf);
        if (tile_tex) {
            return AssetManager_load_sprite_from_texture(m, game, id, tile_tex, src);
        }
    }
    SDL_FreeSurface(surf);
    SDL_FreeSurface(tile_surf);

    return -3;
}


int AssetManager_create_animation(struct AssetManager* m, int id, bool loop)
{
    for (int i = 0; i < MAX_ANIMATIONS; i++)
    {
        if (m->animations[i].id == -1) {
            m->animations[i].id = id;
            m->animations[i].loop = loop;
            printf("%d\n", id);
            return 0;
        }
    }

    return -1;
}


int AssetManager_animation_add_sprite(struct AssetManager* m, int anim_id, int sprite_id, unsigned int duration)
{
    for (int i = 0; i < MAX_ANIMATIONS; i++)
    {
        if (m->animations[i].id == anim_id) {
            struct A_Animation* anim = m->animations + i;
            
            if (anim->len >= MAX_ANIMATION_SIZE) {
                return -1;
            }

            //append sprite
            anim->sprites[anim->len] = sprite_id;
            anim->durations[anim->len] = duration;
            anim->len++;

            printf("%d, %u\n", anim->sprites[anim->len-1], anim->durations[anim->len-1]);
            return 0;
        }
    }
    
    return -1;
}