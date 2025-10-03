#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

#define AL_DEFAULT_CAP 8

ArrayList *arraylist_create(size_t initial_capacity) {
    ArrayList *al = malloc(sizeof(ArrayList));
    if (!al) return NULL;
    if (initial_capacity == 0) initial_capacity = AL_DEFAULT_CAP;
    al->items = calloc(initial_capacity, sizeof(void*));
    if (!al->items) { free(al); return NULL; }
    al->capacity = initial_capacity;
    al->size = 0;
    return al;
}

void arraylist_free(ArrayList *al, void (*free_item)(void*)) {
    if (!al) return;
    if (free_item) {
        for (size_t i = 0; i < al->size; ++i) {
            if (al->items[i]) free_item(al->items[i]);
        }
    }
    free(al->items);
    free(al);
}

static int arraylist_resize(ArrayList *al, size_t newcap) {
    void **tmp = realloc(al->items, newcap * sizeof(void*));
    if (!tmp) return -1;
    al->items = tmp;
    al->capacity = newcap;
    return 0;
}

int arraylist_add(ArrayList *al, void *item) {
    if (!al) return -1;
    if (al->size >= al->capacity) {
        if (arraylist_resize(al, al->capacity * 2) != 0) return -1;
    }
    al->items[al->size++] = item;
    return 0;
}

void *arraylist_get(ArrayList *al, size_t index) {
    if (!al || index >= al->size) return NULL;
    return al->items[index];
}

size_t arraylist_size(ArrayList *al) {
    return al ? al->size : 0;
}

