#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

void to_upper(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = toupper((unsigned char)str[i]);
}

int main() {
    char *fifo1 = "/tmp/fifo1";
    char *fifo2 = "/tmp/fifo2";

    mkfifo(fifo1, 0666);
    mkfifo(fifo2, 0666);

    pid_t pid = fork();

    if (pid == 0) {
        // --- Нащадок ---
        int fd_read = open(fifo1, O_RDONLY);
        int fd_write = open(fifo2, O_WRONLY);

        char buf[100];
        while (1) {
            read(fd_read, buf, sizeof(buf));
            if (strcmp(buf, "stop") == 0) break;
            to_upper(buf);
            write(fd_write, buf, strlen(buf) + 1);
        }

        close(fd_read);
        close(fd_write);
        unlink(fifo1);
        unlink(fifo2);
        exit(0);
    } else {
        // --- Батьківський ---
        int fd_write = open(fifo1, O_WRONLY);
        int fd_read = open(fifo2, O_RDONLY);

        char buf[100];
        while (1) {
            printf("Введіть текст: ");
            fgets(buf, sizeof(buf), stdin);
            buf[strcspn(buf, "\n")] = 0;
            write(fd_write, buf, strlen(buf) + 1);
            if (strcmp(buf, "stop") == 0) break;
            read(fd_read, buf, sizeof(buf));
            printf("Отримано від нащадка: %s\n", buf);
        }

        close(fd_read);
        close(fd_write);
    }
    return 0;
}

