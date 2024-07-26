#include "C_damage.h"
#include "game.h"

void C_Damage_take_damage(struct C_Damage* obj_damage,
                          struct Game* game,
                          int damage)
{
    obj_damage->health -= damage;
    
    if (obj_damage->health <= 0) {
        obj_damage->health = 0;
        Game_send_component_message(game, obj_damage->entity_id,
                                    MESSAGE_DIE);
    }
}