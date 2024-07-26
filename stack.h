/* date = July 23rd 2024 10:30 pm */

#ifndef STACK_H
#define STACK_H


struct Stack
{
    void** items;
    int item_count;
    
    int _max_items;
};


//if base_count == 0 use a default size
void Stack_init(struct Stack* stack, const int base_count);
void Stack_destroy(struct Stack* stack);

void Stack_push(struct Stack* stack, void* item);
void* Stack_pop(struct Stack* stack);
void Stack_print(const struct Stack* stack);

#endif //STACK_H
