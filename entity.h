/* date = June 22nd 2024 0:27 am */

#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>

#define PLAYER_JUMP_FORCE 2.0f

//E_ stands for entity

int E_Player_create(struct Game* game, float x, float y);
void E_Dummy_create(struct Game* game, float x, float y);
void E_Block_create(struct Game* game, float x, float y);
#endif //ENTITY_H
