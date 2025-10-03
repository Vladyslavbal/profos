#ifndef MONITOR_H
#define MONITOR_H

#include <time.h>
#include "arraylist.h"

/* Тип компонента */
typedef enum {
    COMP_PROC,
    COMP_VIDEO,
    COMP_HARD
} component_t;

/* Структура одного запису статистики */
typedef struct {
    time_t timestamp;
    char *hostname;
    char *username;
    component_t component;
    double temperature;
} stat_rec_t;

/* API */
void monitor_init();
void monitor_free();
ArrayList* monitor_collect(component_t *components, size_t comp_count, unsigned int interval_sec, unsigned int iterations);
int monitor_save_to_file(ArrayList *alist, const char *filename);

#endif /* MONITOR_H */

