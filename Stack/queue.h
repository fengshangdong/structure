#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <List/list.h>

//结点链表实现队列
typedef List Queue;

#define queue_init list_init
#define queue_destroy list_destroy

//入队和出队
int queue_enqueue(Queue *queue, const void *data);
int queue_dequeue(Queue *queue, void **data);

#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)
#define queue_size list_size

#endif
