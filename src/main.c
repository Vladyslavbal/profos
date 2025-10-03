#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#include "monitor.h"
#include "arraylist.h"

/* Функція для звільнення записів */
void free_stat(void *p) {
    stat_rec_t *rec = (stat_rec_t*)p;
    if (!rec) return;
    free(rec->hostname);
    free(rec->username);
    free(rec);
}

void usage(const char *prog) {
    printf("Usage: %s [options]\n", prog);
    puts("Options:");
    puts("  -p, --proc         collect CPU temperature");
    puts("  -v, --video        collect GPU temperature");
    puts("  -h, --hard         collect HDD temperature");
    puts("  -t, --time <sec>   interval seconds between iterations (default 1)");
    puts("  -n, --number <n>   number of iterations (default 5)");
    puts("  --help             show this message");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        usage(argv[0]);
        return 0;
    }

    int opt;
    unsigned int interval = 1;
    unsigned int iterations = 5;
    int want_proc = 0, want_video = 0, want_hard = 0;

    struct option longopts[] = {
        {"proc",  no_argument, 0, 'p'},
        {"video", no_argument, 0, 'v'},
        {"hard",  no_argument, 0, 'h'},
        {"time",  required_argument, 0, 't'},
        {"number",required_argument, 0, 'n'},
        {"help",  no_argument, 0,  0 },
        {0,0,0,0}
    };

    while ((opt = getopt_long(argc, argv, "pvh t:n:", longopts, NULL)) != -1) {
        if (opt == 0) {
            /* maybe --help */
            if (strcmp("help", longopts[optind-1].name) == 0) { usage(argv[0]); return 0; }
            continue;
        }
        switch (opt) {
            case 'p': want_proc = 1; break;
            case 'v': want_video = 1; break;
            case 'h': want_hard = 1; break;
            case 't': interval = (unsigned int) atoi(optarg); if (interval == 0) interval = 1; break;
            case 'n': iterations = (unsigned int) atoi(optarg); if (iterations == 0) iterations = 1; break;
            case '?':
            default: usage(argv[0]); return 1;
        }
    }

    /* Якщо не вказано компонентів — повідомити */
    if (!want_proc && !want_video && !want_hard) {
        fprintf(stderr, "No components selected. Use -p, -v, -h.\n");
        usage(argv[0]);
        return 1;
    }

    /* Побудувати список компонентів для збору */
    component_t comps[3];
    size_t comp_count = 0;
    if (want_proc) comps[comp_count++] = COMP_PROC;
    if (want_video) comps[comp_count++] = COMP_VIDEO;
    if (want_hard) comps[comp_count++] = COMP_HARD;

    /* Ініціалізація GPRNG */
    srand((unsigned int) time(NULL) ^ (unsigned int) getpid());

    ArrayList *alist = monitor_collect(comps, comp_count, interval, iterations);
    if (!alist) {
        fprintf(stderr, "Failed to collect stats.\n");
        return 2;
    }

    const char *envfile = getenv("FILE");
    char defname[256];
    if (!envfile || strlen(envfile) == 0) {
        snprintf(defname, sizeof(defname), "./monitor_stats_%ld.csv", (long)time(NULL));
        envfile = defname;
    }

    int rc = monitor_save_to_file(alist, envfile);
    if (rc != 0) {
        fprintf(stderr, "Failed to save to file '%s'\n", envfile);
        arraylist_free(alist, free_stat);
        return 3;
    }

    printf("Saved %zu records to '%s'\n", arraylist_size(alist), envfile);

    arraylist_free(alist, free_stat);
    return 0;
}

