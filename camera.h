/* date = July 1st 2024 6:15 pm */

#ifndef CAMERA_H
#define CAMERA_H
#include "C_transform.h"
#include <stdbool.h>

struct Camera
{
    float x;
    float y;
    struct C_Transform* target;
};


void Camera_follow(struct Camera* camera);
bool Camera_set_target(struct Camera* camera, const struct Game* game, int target_entity_id);


#endif //CAMERA_H
