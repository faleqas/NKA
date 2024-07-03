#include "camera.h"
#include "game.h"


const float MAX_X_SCREEN_WIDTH_FACTOR = 0.6f;
const float MIN_X_SCREEN_WIDTH_FACTOR = 0.2f;


void Camera_follow(struct Camera* camera)
{
    if (!camera) {
        return;
    }
    if (!(camera->target)) {
        return;
    }

    int max_x = ((SCREEN_WIDTH * MAX_X_SCREEN_WIDTH_FACTOR) + camera->x) - camera->target->w;
    int min_x = (SCREEN_WIDTH * MIN_X_SCREEN_WIDTH_FACTOR) + camera->x;

    if (camera->target->x > max_x) {
        int diff = (camera->target->x - max_x);
        camera->x += diff;
    }
    else if (camera->target->x < min_x) {
        int diff = (min_x - camera->target->x);
        camera->x -= diff;
    }

    // if (camera->target) {
    //     camera->x = camera->target->x - ((SCREEN_WIDTH / 2) - OFFSET_FROM_CENTER);
    // }
}

bool Camera_set_target(struct Camera* camera, const struct Game* game, int target_entity_id)
{
    if (!camera) {
        return false;
    }

    if (target_entity_id < 0) {
        util_assert(false, "");
        return false;
    }

    const struct C_Transform* target = game->transforms + target_entity_id;
    camera->target = target;

    return true;
}