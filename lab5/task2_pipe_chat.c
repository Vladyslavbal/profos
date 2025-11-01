#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

void to_upper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

int main() {
    int p1[2], p2[2];
    pipe(p1); // батько -> син
    pipe(p2); // син -> батько

    pid_t pid = fork();

    if (pid == 0) {
        // --- Дочірній процес ---
        close(p1[1]); // не пише в p1
        close(p2[0]); // не читає з p2

        char buffer[100];
        while (1) {
            read(p1[0], buffer, sizeof(buffer));
            buffer[strcspn(buffer, "\n")] = 0; // видаляємо \n
            if (strcmp(buffer, "stop") == 0) break;
            to_upper(buffer);
            write(p2[1], buffer, strlen(buffer) + 1);
        }
        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else {
        // --- Батьківський процес ---
        close(p1[0]); // не читає з p1
        close(p2[1]); // не пише в p2

        char buffer[100];
        while (1) {
            printf("Введіть текст: ");
            fgets(buffer, sizeof(buffer), stdin);
            write(p1[1], buffer, strlen(buffer) + 1);

            buffer[strcspn(buffer, "\n")] = 0;
            if (strcmp(buffer, "stop") == 0) break;

            read(p2[0], buffer, sizeof(buffer));
            printf("Отримано від нащадка: %s\n", buffer);
        }
        close(p1[1]);
        close(p2[0]);
    }
    return 0;
}

