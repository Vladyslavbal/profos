#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define DEFAULT_ARRAY_SIZE 20
#define DEFAULT_WRITERS 2
#define DEFAULT_READERS 5
#define DEFAULT_RUNTIME 10  // seconds

int *array;
int array_size;
int stop_flag = 0;

pthread_rwlock_t rwlock;

// генератор псевдовипадкових чисел у діапазоні [0, max)
int rnd(int max) {
    return rand() % max;
}

// потік-письменник
void* writer_thread(void* arg) {
    long id = (long)arg;
    while (!stop_flag) {
        int index = rnd(array_size);
        int value = rnd(1000);

        pthread_rwlock_wrlock(&rwlock);
        array[index] = value;
        printf("[Writer %ld] -> array[%d] = %d\n", id, index, value);
        pthread_rwlock_unlock(&rwlock);

        usleep(1000 + rnd(200000)); // 1–200 ms
    }
    return NULL;
}

// потік-читач
void* reader_thread(void* arg) {
    long id = (long)arg;
    while (!stop_flag) {
        int index = rnd(array_size);

        pthread_rwlock_rdlock(&rwlock);
        int value = array[index];
        pthread_rwlock_unlock(&rwlock);

        printf("[Reader %ld] <- array[%d] = %d\n", id, index, value);
        usleep(1000 + rnd(150000)); // 1–150 ms
    }
    return NULL;
}

// потік-монітор (виводить масив періодично)
void* monitor_thread(void* arg) {
    while (!stop_flag) {
        pthread_rwlock_rdlock(&rwlock);
        printf("\n[Monitor] Array state:\n");
        for (int i = 0; i < array_size; i++) {
            printf("%4d ", array[i]);
        }
        printf("\n\n");
        pthread_rwlock_unlock(&rwlock);
        sleep(2);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    array_size = DEFAULT_ARRAY_SIZE;
    int num_writers = DEFAULT_WRITERS;
    int num_readers = DEFAULT_READERS;
    int runtime = DEFAULT_RUNTIME;

    // Параметр командного рядка: -n <розмір_масиву>
    if (argc >= 3 && strcmp(argv[1], "-n") == 0) {
        array_size = atoi(argv[2]);
    }

    array = calloc(array_size, sizeof(int));
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t writers[num_writers], readers[num_readers], monitor;

    // створення потоків
    for (long i = 0; i < num_writers; i++)
        pthread_create(&writers[i], NULL, writer_thread, (void*)i);

    for (long i = 0; i < num_readers; i++)
        pthread_create(&readers[i], NULL, reader_thread, (void*)i);

    pthread_create(&monitor, NULL, monitor_thread, NULL);

    // головний потік чекає заданий час
    sleep(runtime);
    stop_flag = 1;

    // приєднання потоків
    for (int i = 0; i < num_writers; i++)
        pthread_join(writers[i], NULL);

    for (int i = 0; i < num_readers; i++)
        pthread_join(readers[i], NULL);

    pthread_join(monitor, NULL);
    pthread_rwlock_destroy(&rwlock);
    free(array);

    printf("\nProgram finished successfully.\n");
    return 0;
}

