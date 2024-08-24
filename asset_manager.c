#include "asset_manager.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_gpu.h>
#include "game.h"
#include "util.h"
#include <stdio.h>
#include <memory.h>

//used internally only
static int _AssetManager_load_sprite(struct AssetManager* m, const struct Game* game,
                                                 int id, GPU_Image* texture, const SDL_Rect* src);

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


static int _AssetManager_load_sprite(struct AssetManager* m, const struct Game* game,
                                                 int id, GPU_Image* texture, SDL_Surface* surface, const SDL_Rect* src)
{
    for (int i = 0; i < MAX_SPRITES; i++)
    {
        if (m->sprites[i].id == -1) {
            //its free
            m->sprites[i].id = id;
            m->sprites[i].texture = texture;
            
            if (src) {
                GPU_Rect* dst = &(m->sprites[i].src);
                dst->x = (float)src->x;
                dst->y = (float)src->y;
                dst->w = (float)src->w;
                dst->h = (float)src->h;
            }
            else {      
                m->sprites[i].src.x = 0;
                m->sprites[i].src.y = 0;
                
                int w;
                int h;
                //SDL_QueryTexture(texture, NULL, NULL,
                //                 w, h
                m->sprites[i].src.w = (float)texture->w;
                m->sprites[i].src.h = (float)texture->h;
            }
            return 0;
        }
    }
    
    return -1;
}


int AssetManager_load_sprite(struct AssetManager* m, const struct Game* game, const char* path,
                             int id, const SDL_Rect* src)
{
    SDL_Surface* surface = IMG_Load(path);
    util_assert(surface != NULL, "A_Sprite: Loading sprite failed. Path is probably invalid\n");
    GPU_Image* texture = GPU_CopyImageFromSurface(surface);
    util_assert(texture != NULL, "A_Sprite: Loading sprite failed. Path is probably invalid\n");
    
    return _AssetManager_load_sprite(m, game, id, texture, surface, src);
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
    util_assert(surf != NULL, "A_Sprite: Loading sprite failed. Path is probably invalid\n");
  
    
    int tile_width = surf->w / tile_count_x;
    int tile_height = surf->h / tile_count_y;
    int x = tile_x * tile_width;
    int y = tile_y * tile_height;
    //printf("%d, %d\n", tile_width, tile_height);
    
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
        GPU_Image* tile_tex = GPU_CopyImageFromSurface(tile_surf);
        util_assert(tile_tex != NULL, "A_Sprite: Loading sprite failed. Path is probably invalid\n");
        if (tile_tex) {
            return _AssetManager_load_sprite(m, game, id, tile_tex, tile_surf, src);
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
            //printf("%d\n", id);
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
            
            //printf("%d, %u\n", anim->sprites[anim->len-1], anim->durations[anim->len-1]);
            return 0;
        }
    }
    
    return -1;
}