/* date = June 25th 2024 11:20 pm */

#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H
#include "game.h"
#include "A_animation.h"
#include <SDL2/SDL.h>

//for now we have a MAX_ number of each asset type so that we don't have to dynamically reallocate everytime we add an asset. if we need to do that in the future we can reallocate by big chunks to improve performance
#define MAX_SPRITES 100
#define MAX_ANIMATIONS 100

//A_ stands for asset

//https://rvros.itch.io/animated-pixel-hero?download


const enum
{
    SPRITE_NONE,
    
    SPRITE_FOREST_BG,
    
    SPRITE_PLAYER_IDLE_00,
    SPRITE_PLAYER_IDLE_01,
    SPRITE_PLAYER_IDLE_02,
    SPRITE_PLAYER_IDLE_03,
    
    SPRITE_PLAYER_RUN_00,
    SPRITE_PLAYER_RUN_01,
    SPRITE_PLAYER_RUN_02,
    SPRITE_PLAYER_RUN_03,
    SPRITE_PLAYER_RUN_04,
    SPRITE_PLAYER_RUN_05,
    
    SPRITE_PLAYER_JUMP_0,
    SPRITE_PLAYER_JUMP_1,
    SPRITE_PLAYER_JUMP_2,
    SPRITE_PLAYER_JUMP_3,
    SPRITE_PLAYER_JUMP_4,
    SPRITE_PLAYER_JUMP_5,
    SPRITE_PLAYER_JUMP_6,
    SPRITE_PLAYER_JUMP_7,
    
    SPRITE_PLAYER_FALLING_0,
    SPRITE_PLAYER_FALLING_1,
    
    SPRITE_PLAYER_SWORD_ATTACK_0_0,
    SPRITE_PLAYER_SWORD_ATTACK_0_1,
    SPRITE_PLAYER_SWORD_ATTACK_0_2,
    SPRITE_PLAYER_SWORD_ATTACK_0_3,
    SPRITE_PLAYER_SWORD_ATTACK_0_4,
    SPRITE_PLAYER_SWORD_ATTACK_0_5,
    SPRITE_PLAYER_SWORD_ATTACK_0_6,
    SPRITE_PLAYER_SWORD_ATTACK_0_7,
    SPRITE_PLAYER_SWORD_ATTACK_0_8,

    SPRITE_PLAYER_WALL_SLIDE_0,
    SPRITE_PLAYER_WALL_SLIDE_1,
    
    SPRITE_DIRT
};

struct A_Sprite
{
    int id;
    SDL_Texture* texture;
    SDL_Rect src;
};

struct AssetManager
{
    struct A_Sprite sprites[MAX_SPRITES];
    struct A_Animation animations[MAX_ANIMATIONS];
};


struct AssetManager* AssetManager_create();

void AssetManager_destroy(struct AssetManager* m);

int AssetManager_create_animation(struct AssetManager* m, int id, bool loop);

//adds a sprite id to the end of the animation
//returns 0 on success, -1 if there is no space to add the sprite (MAX_ANIMATION_SIZE reached)
int AssetManager_animation_add_sprite(struct AssetManager* m, int anim_id, int sprite_id, unsigned int duration);

struct A_Animation* AssetManager_get_animation(const struct AssetManager* m, int id);

struct A_Sprite* AssetManager_get_sprite(const struct AssetManager* m, int id);

//returns -1 if failed
int AssetManager_load_sprite(struct AssetManager* m, const struct Game* game, const char* path,
                             int id, const SDL_Rect* src);

//src here is for the source rect of the sprite itself after its cut from the tilemap for extra options
//since maybe you want to take a certain part of the sprite
int AssetManager_load_sprite_from_tilemap(struct AssetManager* m, const struct Game* game, const char* path,
                                          int id, unsigned int tile_x, unsigned int tile_y,
                                          unsigned int tile_count_x, unsigned tile_count_y,
                                          const SDL_Rect* src);

#endif //ASSET_MANAGER_H
