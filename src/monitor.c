#define _POSIX_C_SOURCE 200809L
#include "monitor.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <sys/utsname.h>
#include <time.h>
#include <errno.h>

/* Внутрішні допоміжні */
static char *dup_or_null(const char *s) {
    if (!s) return NULL;
    char *r = strdup(s);
    return r;
}

static double rand_double_range(double lo, double hi) {
    double r = (double)rand() / (double)RAND_MAX;
    return lo + r * (hi - lo);
}

ArrayList* monitor_collect(component_t *components, size_t comp_count, unsigned int interval_sec, unsigned int iterations) {
    if (!components || comp_count == 0 || iterations == 0) return NULL;

    ArrayList *alist = arraylist_create( (size_t) (comp_count * iterations) );
    if (!alist) return NULL;

    /* Отримати hostname та username (копії) */
    char hbuf[256];
    if (gethostname(hbuf, sizeof(hbuf)) != 0) hbuf[0] = '\0';
    char *hostname = dup_or_null(hbuf);

    char *login = getlogin();
    char *username = NULL;
    if (login) username = dup_or_null(login);
    else {
        uid_t uid = getuid();
        struct passwd *pw = getpwuid(uid);
        if (pw && pw->pw_name) username = dup_or_null(pw->pw_name);
    }
    if (!username) username = strdup("unknown");

    for (unsigned int iter = 0; iter < iterations; ++iter) {
        for (size_t ci = 0; ci < comp_count; ++ci) {
            component_t comp = components[ci];
            double temp = 0.0;
            switch (comp) {
                case COMP_PROC: temp = rand_double_range(30.0, 100.0); break;
                case COMP_VIDEO: temp = rand_double_range(30.0, 120.0); break;
                case COMP_HARD: temp = rand_double_range(30.0, 50.0); break;
                default: temp = 0.0; break;
            }

            stat_rec_t *rec = malloc(sizeof(stat_rec_t));
            if (!rec) continue;
            rec->timestamp = time(NULL);
            rec->hostname = dup_or_null(hostname);
            rec->username = dup_or_null(username);
            rec->component = comp;
            rec->temperature = temp;

            if (arraylist_add(alist, rec) != 0) {
                free(rec->hostname); free(rec->username); free(rec);
            }
        }

        if (iter + 1 < iterations) {
            sleep(interval_sec);
        }
    }

    free(hostname);
    free(username);
    return alist;
}

int monitor_save_to_file(ArrayList *alist, const char *filename) {
    if (!alist || !filename) return -1;
    FILE *f = fopen(filename, "w");
    if (!f) return -1;

    /* Заголовок CSV */
    fprintf(f, "timestamp_iso,unix_ts,hostname,username,component,temperature\n");

    size_t n = arraylist_size(alist);
    for (size_t i = 0; i < n; ++i) {
        stat_rec_t *rec = (stat_rec_t*) arraylist_get(alist, i);
        if (!rec) continue;
        char tstr[64];
        struct tm tm;
        localtime_r(&rec->timestamp, &tm);
        strftime(tstr, sizeof(tstr), "%Y-%m-%dT%H:%M:%S%z", &tm);

        const char *comp_str = "unknown";
        switch (rec->component) {
            case COMP_PROC: comp_str = "proc"; break;
            case COMP_VIDEO: comp_str = "video"; break;
            case COMP_HARD: comp_str = "hard"; break;
        }

        fprintf(f, "%s,%ld,%s,%s,%s,%.3f\n",
                tstr,
                (long)rec->timestamp,
                rec->hostname ? rec->hostname : "",
                rec->username ? rec->username : "",
                comp_str,
                rec->temperature);
    }

    fclose(f);
    return 0;
}

/* Звільнення memory для записів */
void monitor_free() {
    /* Нічого тут — звільнення зробимо через arraylist_free з callback */
}

