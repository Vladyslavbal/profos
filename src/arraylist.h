#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h>

typedef struct ArrayList {
    void **items;
    size_t size;
    size_t capacity;
} ArrayList;

/* Ініціалізація/видалення */
ArrayList *arraylist_create(size_t initial_capacity);
void arraylist_free(ArrayList *al, void (*free_item)(void*)); /* free_item може бути NULL */

/* Операції */
int arraylist_add(ArrayList *al, void *item);
void *arraylist_get(ArrayList *al, size_t index);
size_t arraylist_size(ArrayList *al);

#endif /* ARRAYLIST_H */

