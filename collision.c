#include "collision.h"


bool is_colliding_aabb(const struct C_Transform* trans1,
                      const struct C_Transform* trans2)
{
    if ( (trans1->x < trans2->x + trans2->w) &&
         (trans1->x + trans1->w > trans2->x) &&
         (trans1->y < trans2->y + trans2->h) &&
         (trans1->y + trans1->h > trans2->y)
         )
         {
            return true;
         }

    return false;
}


//refer to this to understand the naming of the dots
//https://cdn.tutsplus.com/cdn-cgi/image/width=550/gamedev/uploads/legacy/008_separatingAxisTheorem/assets/conventions.jpg

bool is_colliding_sat(const struct C_Transform* trans1,
                      const struct C_Transform* trans2)
{
    struct Point dot1_0 = { //center point of 1st
        trans1->x + (trans1->w / 2),
        trans1->y + (trans1->h / 2)
    };
    
    struct Point dot1_1 = { //upper right point of 1st
        trans1->x + trans1->w,
        trans1->y
    };
    
    struct Point dot1_2 = { //lower right point of 1st
        trans1->x + trans1->w,
        trans1->y + trans1->h
    };
    
    struct Point dot1_3 = { //lower left point of 1st
        trans1->x,
        trans1->y + trans1->h
    };
    
    struct Point dot1_4 = { //upper left point of 1st
        trans1->x,
        trans1->y
    };
    
    //second box
    struct Point dot2_0 = { //center point of 2nd
        trans2->x + (trans2->w / 2),
        trans2->y + (trans2->h / 2)
    };
    
    struct Point dot2_1 = { //upper right point of 2nd
        trans2->x + trans2->w,
        trans2->y
    };
    
    struct Point dot2_2 = { //lower right point of 2nd
        trans2->x + trans2->w,
        trans2->y + trans2->h
    };
    
    struct Point dot2_3 = { //lower left point of 2nd
        trans2->x,
        trans2->y + trans2->h
    };
    
    struct Point dot2_4 = { //upper left point of 2nd
        trans2->x,
        trans2->y
    };
    
    { //Y axis
        float box1_min = dot1_1.y;
        float box1_max = dot1_2.y;
        
        float box2_min = dot2_1.y;
        float box2_max = dot2_2.y;
        
        if (box2_min > box1_max) {
            return false;
        }
        else if (box1_min > box2_max) {
            return false;
        }
    }
    
    { //X axis
        float box1_min = dot1_4.x;
        float box1_max = dot1_1.x;
        
        float box2_min = dot2_4.x;
        float box2_max = dot2_1.x;
        
        if (box2_min > box1_max) {
            return false;
        }
        else if (box1_min > box2_max) {
            return false;
        }
    }
    
    return true;
}


float dot_product(float x1, float x2,
                  float y1, float y2)
{
    return (x1 * x2) + (y1 * y2);
}