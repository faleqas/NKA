#include <memory.h>
#include <malloc.h>

#include "queue.h"


void Queue_init(struct Queue* queue, const int base_count)
{
    if (base_count == 0) {
        queue->_max_items = 2;
    }
    else {
        queue->_max_items = base_count;
    }
    
    queue->item_count = 0;
    queue->items = calloc(queue->_max_items, sizeof(void*));
}


void Queue_destroy(struct Queue* queue)
{
    if (queue->items) {
        free(queue->items);
    }
}


void Queue_push(struct Queue* queue, void* item)
{
    queue->item_count++;
    
    if (queue->_max_items <= queue->item_count) {
        queue->_max_items += 4;
        queue->items = realloc(queue->items,
                               sizeof(void*) * queue->_max_items);
    }
    
    queue->items[queue->item_count-1] = item;
}


void* Queue_pop(struct Queue* queue)
{
    if (queue->item_count <= 0) {
        return NULL;
    }
    
    void* item = queue->items[0];
    queue->items[0] = NULL;
    
    for (int i = 0; i < queue->item_count-1; i++)
    {
        queue->items[i] = queue->items[i+1];
    }
    
    queue->item_count--;
    
    return item;
}


void Queue_print(const struct Queue* queue)
{
    for (int i = 0; i < queue->item_count; i++)
    {
        printf("%d: %p\n", i, queue->items[i]);
    }
}