/* date = July 24th 2024 3:19 am */

#ifndef QUEUE_H
#define QUEUE_H


struct Queue
{
    void** items;
    int item_count;
    
    int _max_items;
};


//if base_count == 0 use a default size
void Queue_init(struct Queue* queue, const int base_count);
void Queue_destroy(struct Queue* queue);

void Queue_push(struct Queue* queue, void* item);
void* Queue_pop(struct Queue* queue);
void Queue_print(const struct Queue* queue);


#endif //QUEUE_H
