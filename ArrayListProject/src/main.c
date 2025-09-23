#include <stdio.h>
#include "arraylist.h"

int main() {
    ArrayList list;
    arraylist_init(&list, 2);

    printf("Додаємо елементи 10, 20, 30\n");
    arraylist_add(&list, 10);
    arraylist_add(&list, 20);
    arraylist_add(&list, 30);

    printf("Поточний розмір: %zu\n", arraylist_size(&list));

    printf("Вставка 15 на позицію 1\n");
    arraylist_insert(&list, 1, 15);

    printf("Список: ");
    for (size_t i = 0; i < arraylist_size(&list); i++) {
        printf("%d ", arraylist_get(&list, i));
    }
    printf("\n");

    printf("Змінюємо елемент на позиції 2 на 25\n");
    arraylist_set(&list, 2, 25);

    printf("Видаляємо елемент на позиції 0\n");
    arraylist_remove(&list, 0);

    printf("Список після змін: ");
    for (size_t i = 0; i < arraylist_size(&list); i++) {
        printf("%d ", arraylist_get(&list, i));
    }
    printf("\n");

    arraylist_free(&list);
    return 0;
}

