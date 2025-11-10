#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define THREAD_COUNT 4  // кількість потоків
#define N 1000000       // кількість підінтервалів

// межі інтегрування
double a = 0.0;
double b = 2.0;

// глобальна змінна для результату
double global_result = 0.0;

// м’ютекс для захисту спільного ресурсу
pthread_mutex_t mutex;

// функція для обчислення f(x)
double f(double x) {
    return 4 - x * x;
}

// структура для передачі параметрів у потік
typedef struct {
    int id;
    int n_threads;
} thread_data;

// функція, яку виконуватиме кожен потік
void* integrate(void* arg) {
    thread_data* data = (thread_data*)arg;
    int id = data->id;
    int n_threads = data->n_threads;

    double h = (b - a) / N;   // крок інтегрування
    int chunk = N / n_threads; // кількість точок на потік

    int start = id * chunk;
    int end = (id == n_threads - 1) ? N : start + chunk;

    double local_sum = 0.0;

    // обчислення частини інтегралу для цього потоку
    for (int i = start; i < end; i++) {
        double x = a + (i + 0.5) * h;  // середина прямокутника
        local_sum += f(x);
    }

    local_sum *= h;

    // критична секція — додавання до спільної змінної
    pthread_mutex_lock(&mutex);
    global_result += local_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    thread_data data[THREAD_COUNT];

    // ініціалізація м’ютекса
    pthread_mutex_init(&mutex, NULL);

    // створення потоків
    for (int i = 0; i < THREAD_COUNT; i++) {
        data[i].id = i;
        data[i].n_threads = THREAD_COUNT;
        if (pthread_create(&threads[i], NULL, integrate, &data[i]) != 0) {
            perror("Помилка створення потоку");
            return 1;
        }
    }

    // очікування завершення потоків
    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // виведення результату
    printf("Обчислений інтеграл: %.10f\n", global_result);
    printf("Теоретичне значення: 5.3333333333\n");

    // знищення м’ютекса
    pthread_mutex_destroy(&mutex);

    return 0;
}

