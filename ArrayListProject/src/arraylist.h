#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stddef.h> // для size_t

typedef struct {
    int *data;       // динамічний масив елементів
    size_t size;     // поточна кількість елементів
    size_t capacity; // максимальна кількість елементів
} ArrayList;

// Ініціалізація ArrayList
void arraylist_init(ArrayList *list, size_t initial_capacity);

// Звільнення пам’яті
void arraylist_free(ArrayList *list);

// Додавання елемента в кінець списку
void arraylist_add(ArrayList *list, int item);

// Вставка елемента за індексом
void arraylist_insert(ArrayList *list, size_t index, int item);

// Видалення елемента за індексом
void arraylist_remove(ArrayList *list, size_t index);

// Зміна значення елемента
void arraylist_set(ArrayList *list, size_t index, int item);

// Отримання значення елемента
int arraylist_get(ArrayList *list, size_t index);

// Поточний розмір списку
size_t arraylist_size(ArrayList *list);

#endif

