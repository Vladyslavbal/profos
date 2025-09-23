#include "arraylist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void arraylist_init(ArrayList *list, size_t initial_capacity) {
    list->data = (int *)malloc(initial_capacity * sizeof(int));
    if (!list->data) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->size = 0;
    list->capacity = initial_capacity;
}

void arraylist_free(ArrayList *list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void arraylist_resize(ArrayList *list, size_t new_capacity) {
    int *new_data = (int *)realloc(list->data, new_capacity * sizeof(int));
    if (!new_data) {
        fprintf(stderr, "Memory reallocation failed\n");
        exit(EXIT_FAILURE);
    }
    list->data = new_data;
    list->capacity = new_capacity;
}

void arraylist_add(ArrayList *list, int item) {
    if (list->size >= list->capacity) {
        arraylist_resize(list, list->capacity * 2);
    }
    list->data[list->size++] = item;
}

void arraylist_insert(ArrayList *list, size_t index, int item) {
    if (index > list->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    if (list->size >= list->capacity) {
        arraylist_resize(list, list->capacity * 2);
    }
    memmove(&list->data[index + 1], &list->data[index], (list->size - index) * sizeof(int));
    list->data[index] = item;
    list->size++;
}

void arraylist_remove(ArrayList *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    memmove(&list->data[index], &list->data[index + 1], (list->size - index - 1) * sizeof(int));
    list->size--;
}

void arraylist_set(ArrayList *list, size_t index, int item) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    list->data[index] = item;
}

int arraylist_get(ArrayList *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return list->data[index];
}

size_t arraylist_size(ArrayList *list) {
    return list->size;
}

