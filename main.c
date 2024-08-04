// main.c: This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <util.h>
#include <stdbool.h>
#include <stdint.h>
#include "game.h"
#include "component.h"
#include "asset_manager.h"
#include <memory.h>
#include "stack.h"
#include "queue.h"


static struct Game* create_game();
static void destroy_game(struct Game* game);
static void main_loop(struct Game* game);
static int handle_events(struct Game* game);


int main(int argc, char** argv)
{
    struct Game* game = create_game();
    main_loop(game);
    destroy_game(game);
    
    return 0;
}


static void main_loop(struct Game* game)
{
    game->asset_m = AssetManager_create();
    
    { //PLAYER IDLE from (0, 0) to (3, 0) in tilesheet
        SDL_Rect rect = { //size of idle sprites
            16, 6, //offset
            19, 31
        };
        
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_IDLE_00,
                                              0, 0,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_IDLE_01,
                                              1, 0,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_IDLE_02,
                                              2, 0,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_IDLE_03,
                                              3, 0,
                                              7, 16, &rect);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_IDLE_0, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_IDLE_0, SPRITE_PLAYER_IDLE_00, 150);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_IDLE_0, SPRITE_PLAYER_IDLE_01, 150);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_IDLE_0, SPRITE_PLAYER_IDLE_02, 150);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_IDLE_0, SPRITE_PLAYER_IDLE_03, 150);
    }

    { //PLAYER RUN from (1,1) to (5,1)
        SDL_Rect rect = { //size of run sprites
            16, 6, //offset
            21, 33
        };
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_RUN_00,
                                              1, 1,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_RUN_01,
                                              2, 1,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_RUN_02,
                                              3, 1,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_RUN_03,
                                              4, 1,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_RUN_04,
                                              5, 1,
                                              7, 16, &rect);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_RUN, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_RUN, SPRITE_PLAYER_RUN_00, 120);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_RUN, SPRITE_PLAYER_RUN_01, 120);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_RUN, SPRITE_PLAYER_RUN_02, 120);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_RUN, SPRITE_PLAYER_RUN_03, 120);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_RUN, SPRITE_PLAYER_RUN_04, 120);
    }

    {
        SDL_Rect rect = { //size of jump sprites
            16, 6, //offset
            19, 31
        };
        
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_0,
                                              0, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_1,
                                              1, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_2,
                                              2, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_3,
                                              3, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_4,
                                              4, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_5,
                                              5, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_6,
                                              6, 2,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_JUMP_7,
                                              0, 3,
                                              7, 16, &rect);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_AIR_JUMP, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_AIR_JUMP, SPRITE_PLAYER_JUMP_4, 70);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_AIR_JUMP, SPRITE_PLAYER_JUMP_5, 70);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_AIR_JUMP, SPRITE_PLAYER_JUMP_6, 70);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_AIR_JUMP, SPRITE_PLAYER_JUMP_7, 70);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_READY_JUMP, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_READY_JUMP, SPRITE_PLAYER_JUMP_0, 100);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_READY_JUMP, SPRITE_PLAYER_JUMP_1, 100);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_FIRST_JUMP, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_FIRST_JUMP, SPRITE_PLAYER_JUMP_2, 200);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_FIRST_JUMP, SPRITE_PLAYER_JUMP_3, 200);
    }

    {
        SDL_Rect rect = { //size of falling sprites
            16, 6, //offset
            19, 31
        };
        
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_FALLING_0,
                                              1, 3,
                                              7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_FALLING_1,
                                              2, 3,
                                              7, 16, &rect);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_FALLING, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_FALLING, SPRITE_PLAYER_FALLING_0, 200);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_FALLING, SPRITE_PLAYER_FALLING_1, 200);
    }

    {
        SDL_Rect rect1 = { //size of idle sword sprites
            50 - 34 - 5, 0, //offset
            34, 36
        };
        
        SDL_Rect rect2 = { //size of crouch sword sprites
            50 - 34 - 10, 0, //offset
            34, 36
        };
        
        SDL_Rect rect3 = { //size of sword attack sprites
            50 - 34, 0, //offset
            34, 36
        };
        
        //(0, 6), (1, 6), (2, 6)
        
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_0,
                                              3, 5,
                                              7, 16, &rect1);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_1,
                                              4, 5,
                                              7, 16, &rect1);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_2,
                                              5, 5,
                                              7, 16, &rect1);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_3,
                                              6, 5,
                                              7, 16, &rect1);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_4,
                                              0, 6,
                                              7, 16, &rect2);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_5,
                                              1, 6,
                                              7, 16, &rect2);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_6,
                                              2, 6,
                                              7, 16, &rect3);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_7,
                                              3, 6,
                                              7, 16, &rect3);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
                                              SPRITE_PLAYER_SWORD_ATTACK_0_8,
                                              4, 6,
                                              7, 16, &rect3);
        
        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_ATTACK_0, false);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_0, 20);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_1, 20);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_2, 30);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_3, 30);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_4, 40);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_5, 60);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_6, 100);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_7, 80);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_ATTACK_0, SPRITE_PLAYER_SWORD_ATTACK_0_8, 60);
    }

    //2 3
    //11
    {
        SDL_Rect rect = { //size of wall slide sprites
            14, 6, //offset
            19, 31
        };

        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
            SPRITE_PLAYER_WALL_SLIDE_0,
            2, 11,
            7, 16, &rect);
        AssetManager_load_sprite_from_tilemap(game->asset_m, game, "assets/sprites/adventurer/sheet_edit.png",
            SPRITE_PLAYER_WALL_SLIDE_1,
            3, 11,
            7, 16, &rect);

        AssetManager_create_animation(game->asset_m, ANIM_PLAYER_WALL_SLIDE, true);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_WALL_SLIDE, SPRITE_PLAYER_WALL_SLIDE_0, 200);
        AssetManager_animation_add_sprite(game->asset_m, ANIM_PLAYER_WALL_SLIDE, SPRITE_PLAYER_WALL_SLIDE_1, 200);
    }
    
    AssetManager_load_sprite(game->asset_m, game, "assets/sprites/world1/individual/dirt0.png",
                             SPRITE_DIRT, NULL);
    
    struct A_Sprite* s_background = NULL;
    {
        // int id = AssetManager_load_sprite(game->asset_m, game, "assets/sprites/bg.png",
        //                      SPRITE_FOREST_BG, NULL);
        // s_background = AssetManager_get_sprite(game->asset_m, SPRITE_FOREST_BG);
    }
    
    game->e_player_id = E_Player_create(game, 90, TILE_SIZE * 7);
    Camera_set_target(&(game->camera), game, game->e_player_id);
    
    E_Dummy_create(game, 400, TILE_SIZE * 7.5f);
    
    for (int i = 0; i < SCREEN_WIDTH * 2; i += TILE_SIZE)
    {
        E_Block_create(game, (float)i, (float)TILE_SIZE * 9);
    }
    // for (int i = 0; i < SCREEN_WIDTH; i += (TILE_SIZE * 2 + 20))
    // {
    //     E_Block_create(game, (float)i, (float)TILE_SIZE * 6);
    // }
    for (int i = 0; i < SCREEN_HEIGHT- 100; i += TILE_SIZE)
    {
        E_Block_create(game, (float)0, (float)i);
    }
    
    int last_ticks = SDL_GetTicks();
    game->tics = 0;
    while (true)
    {
        if (SDL_GetTicks() - last_ticks < 1000/DESIRED_FPS) {
            continue;
        }
        last_ticks = SDL_GetTicks();
        
        { //clear screen
            SDL_SetRenderDrawColor(game->renderer,
                                   60, 60, 60, 255);
            SDL_RenderClear(game->renderer);
        }
        if (s_background) {
            SDL_Rect dst = {0, -120, SCREEN_WIDTH + 120, SCREEN_HEIGHT + 120};
            SDL_RenderCopyEx(game->renderer, s_background->texture,
                             &(s_background->src), &dst,
                             0, NULL,
                             SDL_FLIP_NONE);
        }
        
        if (handle_events(game)) {
            return;
        }
        
        for (int i = 0; i < ENTITY_MAX; i++)
        {
            if (game->states[i].entity_id != -1)
            {
                C_State_update(game->states + i, game);
            }
        }
        for (int i = 0; i < ENTITY_MAX; i++)
        {
            if (game->movements[i].entity_id != -1)
            {
                C_Movement_update(game->movements + i, game);
            }
        }
        for (int i = 0; i < ENTITY_MAX; i++)
        {
            if (game->melees[i].entity_id != -1)
            {
                C_Melee_update(game->melees + i, game);
            }
        }
        
        Camera_follow(&(game->camera));
        for (int i = 0; i < ENTITY_MAX; i++)
        {
            if (game->draws[i].entity_id != -1)
            {
                C_Draw_update(game->draws + i, game);
            }
        }
        
        SDL_RenderPresent(game->renderer);
        
        game->tics++;
    }
}


static int handle_events(struct Game* game)
{
    memset(game->keys_just_pressed, 0, sizeof(uint8_t) * game->key_count);
    memset(game->keys_just_released, 0, sizeof(uint8_t) * game->key_count);
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                return 1;
            } break;
            
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_p:
                    {
                        game->draw_collisions = !(game->draw_collisions);
                    } break;
                }
                uint8_t scancode = event.key.keysym.scancode;

                game->keys_held[scancode] = 1;

                if (event.key.repeat == 0) {
                    game->keys_just_pressed[scancode] = 1;
                    printf("%d just pressed\n", scancode);
                }
                
            } break;
            
            case SDL_KEYUP:
            {
                uint8_t scancode = event.key.keysym.scancode;
                game->keys_just_released[scancode] = 1;
                game->keys_held[scancode] = 0;
            } break;
        }
    };
    
    return 0;
}


static struct Game* create_game()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    SDL_Window* window = SDL_CreateWindow("NKA",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          0);
    util_assert(window != NULL, "SDL_Window creation failed\n");
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0,
                                                SDL_RENDERER_ACCELERATED);
    
    struct Game* game = calloc(1, sizeof(struct Game));
    util_assert(game != NULL, ""); //never happens but keep compiler happy
    game->renderer = renderer;
    game->window = window;
    game->entity_count = 0;
    game->draw_collisions = false;
    
    //components
    for (int i = 0; i < ENTITY_MAX; i++)
    {
        game->transforms[i].entity_id = -1;
        game->movements[i].entity_id = -1;
        game->draws[i].entity_id = -1;
        game->states[i].entity_id = -1;
        game->melees[i].entity_id = -1;
        game->damages[i].entity_id = -1;
    }
    
    game->camera.x = 0;
    game->camera.y = 0;
    game->camera.target = NULL;
    
    game->e_player_id = -1;
    
    //initialize input state crap
    
    
    SDL_GetKeyboardState(&(game->key_count));
    util_assert(game->key_count, "");
    
    game->keys_just_pressed = calloc(game->key_count, sizeof(uint8_t));
    game->keys_just_released = calloc(game->key_count, sizeof(uint8_t));
    game->keys_held = calloc(game->key_count, sizeof(uint8_t));

    game->message_memory_max = 64;
    game->message_memory = malloc(sizeof(struct Message) * game->message_memory_max);

    for (int i = 0; i < game->message_memory_max; i++)
    {
        game->message_memory[i].id = -1;
    }
    
    return game;
}


static void destroy_game(struct Game* game)
{
    util_assert(game != NULL, "");
    IMG_Quit();
    SDL_DestroyWindow(game->window);
    SDL_Quit();
    AssetManager_destroy(game->asset_m);
    
    //input state
    free(game->keys_just_pressed);
    
    free(game);
}