#include <memory.h>
#include <malloc.h>

#include "stack.h"



void Stack_init(struct Stack* stack, const int base_count)
{
    if (base_count == 0) {
        stack->_max_items = 2;
    }
    else {
        stack->_max_items = base_count;
    }
    
    stack->item_count = 0;
    stack->items = calloc(stack->_max_items, sizeof(void*));
}


void Stack_destroy(struct Stack* stack)
{
    if (stack->items) {
        free(stack->items);
    }
}


void Stack_push(struct Stack* stack, void* item)
{
    stack->item_count++;
    
    if (stack->_max_items <= stack->item_count) {
        stack->_max_items += 4;
        stack->items = realloc(stack->items,
                               sizeof(void*) * stack->_max_items);
    }
    
    stack->items[stack->item_count-1] = item;
}


void* Stack_pop(struct Stack* stack)
{
    if (stack->item_count <= 0) {
        return NULL;
    }
    
    stack->item_count--;
    void* item = stack->items[stack->item_count];
    
    stack->items[stack->item_count] = NULL;
    return item;
}


void Stack_print(const struct Stack* stack)
{
    for (int i = 0; i < stack->item_count; i++)
    {
        printf("%d: %p\n", i, stack->items[i]);
    }
}