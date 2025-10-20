#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

void print_help() {
    printf("Використання: mycat [опції] <вхідний_файл>\n");
    printf("Опції:\n");
    printf("  -h, --help        Вивести це повідомлення та вийти\n");
    printf("  -o, --output <file> Записати результат у вказаний файл\n");
    printf("  -p, --position <n>  Починати читання з позиції n (байт)\n");
    printf("  -c, --count <n>   Зчитати n байтів (за замовчуванням до кінця файлу)\n");
    printf("  -n, --newline  Закінчити вивід символом нового рядка\n");
}

int main(int argc, char *argv[]) {
    int option_index = 0;
    int opt;
    
    char *output_file = NULL;
    long position = 0;
    long count = -1; // -1 означає "до кінця"
    int newline = 0;
    
    struct option long_options[] = {
        {"help",     no_argument,       0, 'h'},
        {"output",   required_argument, 0, 'o'},
        {"position", required_argument, 0, 'p'},
        {"count",    required_argument, 0, 'c'},
        {"newline",  no_argument,       0, 'n'},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "ho:p:c:n", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'o':
                output_file = optarg;
                break;
            case 'p':
                position = atol(optarg);
                break;
            case 'c':
                count = atol(optarg);
                break;
            case 'n':
                newline = 1;
                break;
            default:
                fprintf(stderr, "Невідома опція. Використайте -h для допомоги.\n");
                return 1;
        }
    }

    // Перевірка, чи вказано ім'я вхідного файлу
    if (optind >= argc) {
        fprintf(stderr, "Помилка: не вказано ім'я вхідного файлу.\n");
        fprintf(stderr, "Спробуйте: mycat -h\n");
        return 1;
    }

    char *input_file = argv[optind];

    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("Помилка відкриття вхідного файлу");
        return 1;
    }

    FILE *out = stdout;
    if (output_file) {
        out = fopen(output_file, "wb");
        if (!out) {
            perror("Помилка відкриття вихідного файлу");
            fclose(in);
            return 1;
        }
    }

    // Переміщення на потрібну позицію
    if (position > 0) {
        if (fseek(in, position, SEEK_SET) != 0) {
            perror("Помилка переміщення у файлі");
            fclose(in);
            if (out != stdout) fclose(out);
            return 1;
        }
    }

    char buffer[1024];
    long bytes_to_read = count;
    while (1) {
        size_t chunk_size;
        if (bytes_to_read < 0) {
            chunk_size = sizeof(buffer);
        } else {
            if (bytes_to_read <= 0) break;
            chunk_size = (bytes_to_read < (long)sizeof(buffer)) ? bytes_to_read : sizeof(buffer);
        }

        size_t bytes_read = fread(buffer, 1, chunk_size, in);
        if (bytes_read == 0) break;

        fwrite(buffer, 1, bytes_read, out);

        if (bytes_to_read > 0) {
            bytes_to_read -= bytes_read;
        }
    }

    if (newline) {
        fputc('\n', out);
    }

    fclose(in);
    if (out != stdout) fclose(out);

    return 0;
}

