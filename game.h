/* date = June 22nd 2024 0:28 am */

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "entity.h"
#include "component.h"
#include <stdbool.h>
#include "C_draw.h"
#include "camera.h"

#define ENTITY_MAX 10000
#define DESIRED_FPS 240
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TILE_SIZE 64
#define GRAVITY 0.02f
#define MAX_GRAVITY 6.0f


//Component messages
enum
{
    MESSAGE_NONE,
    MESSAGE_MATTACK_DAMAGE_FRAME, //this is the frame where u should do damage in the melee attack

    MESSAGE_FRAME_ADVANCE, //animation
    
    MESSAGE_DIE
};

struct Message
{
    int id;
    int lvalue;
    int rvalue;

    int _index; //index in message_memory array
};


struct Game
{
    int tics;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    struct Camera camera;
    
    int entity_count;
    int e_player_id;
    
    //input stuff
    int key_count;
    uint8_t* keys_just_pressed;
    uint8_t* keys_just_released;
    uint8_t* keys_held;
    //uint8_t* prev_keys; //key state from the previous frame
    
    //managers
    struct AssetManager* asset_m;
    
    //components
    struct C_Transform transforms[ENTITY_MAX];
    struct C_Movement movements[ENTITY_MAX];
    struct C_Draw draws[ENTITY_MAX];
    struct C_State states[ENTITY_MAX];
    struct C_Melee melees[ENTITY_MAX];
    struct C_Damage damages[ENTITY_MAX];
    
    //debug
    bool draw_collisions;

    struct Message* message_memory;
    int message_memory_max;
    int message_index;
};


//returns true if ENTITY_MAX has been reached
bool Game_is_entities_full(const struct Game* game);

struct C_Transform* Game_is_colliding(const struct Game* game,
                                      const struct C_Transform* t);

void Game_clear_entity(struct Game* game, int entity_id);

struct Message* Game_create_message(struct Game* game, int id, int lvalue, int rvalue);

void Game_delete_message(struct Game* game, struct Message* msg);

//sends a message to all components of an entity
void Game_send_component_message(struct Game* game,
                                 int entity_id,
                                 const struct Message* msg
                                 );

#endif //GAME_H
