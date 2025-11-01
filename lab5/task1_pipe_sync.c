#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];  // файлові дескриптори для каналу
    int n = 3;  // кількість дочірніх процесів

    if (pipe(fd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0) {
            // Дочірній процес
            close(fd[0]); // закриваємо кінець для читання

            printf("Дочірній процес %d виконує роботу...\n", i + 1);
            sleep(1); // імітація роботи

            // Надсилаємо сигнал батькові
            write(fd[1], "1", 1);
            close(fd[1]);
            exit(0);
        }
    }

    // Батьківський процес
    close(fd[1]); // закриваємо кінець для запису

    char buf;
    for (int i = 0; i < n; i++) {
        read(fd[0], &buf, 1);
    }

    printf("Батьківський процес отримав сигнал від усіх дочірніх!\n");
    close(fd[0]);

    for (int i = 0; i < n; i++) wait(NULL);
    return 0;
}

