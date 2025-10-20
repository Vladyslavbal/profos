#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void print_help() {
    printf("Використання: dir [опції] [каталог]\n");
    printf("Опції:\n");
    printf("  -h, --help       Показати цю допомогу\n");
    printf("  -l, --long       Вивести детальну інформацію про кожен файл\n");
    printf("  -a, --all        Вивести також приховані файли (починаються з .)\n");
}

// Функція для виведення прав доступу у форматі rwxrwxrwx
void print_permissions(mode_t mode) {
    char perms[10];
    perms[0] = (mode & S_IRUSR) ? 'r' : '-';
    perms[1] = (mode & S_IWUSR) ? 'w' : '-';
    perms[2] = (mode & S_IXUSR) ? 'x' : '-';
    perms[3] = (mode & S_IRGRP) ? 'r' : '-';
    perms[4] = (mode & S_IWGRP) ? 'w' : '-';
    perms[5] = (mode & S_IXGRP) ? 'x' : '-';
    perms[6] = (mode & S_IROTH) ? 'r' : '-';
    perms[7] = (mode & S_IWOTH) ? 'w' : '-';
    perms[8] = (mode & S_IXOTH) ? 'x' : '-';
    perms[9] = '\0';
    printf("%s", perms);
}

int main(int argc, char *argv[]) {
    int opt;
    int option_index = 0;
    int show_long = 0;
    int show_all = 0;

    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"long", no_argument, 0, 'l'},
        {"all", no_argument, 0, 'a'},
        {0, 0, 0, 0}
    };

    // Обробка опцій
    while ((opt = getopt_long(argc, argv, "hla", long_options, &option_index)) != -1) {
        switch(opt) {
            case 'h':
                print_help();
                return 0;
            case 'l':
                show_long = 1;
                break;
            case 'a':
                show_all = 1;
                break;
            default:
                fprintf(stderr, "Невідома опція. Використайте -h для допомоги.\n");
                return 1;
        }
    }

    // Позиційний параметр — каталог (за замовчуванням .)
    char *dir_path = ".";
    if (optind < argc) {
        dir_path = argv[optind];
    }

    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("Помилка відкриття каталогу");
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Пропускаємо приховані файли, якщо не вказана опція -a
        if (!show_all && entry->d_name[0] == '.') continue;

        if (show_long) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

            struct stat st;
            if (stat(full_path, &st) == -1) {
                perror("stat");
                continue;
            }

            // Тип файлу
            char type = '?';
            if (S_ISREG(st.st_mode)) type = '-';
            else if (S_ISDIR(st.st_mode)) type = 'd';
            else if (S_ISLNK(st.st_mode)) type = 'l';
            else if (S_ISCHR(st.st_mode)) type = 'c';
            else if (S_ISBLK(st.st_mode)) type = 'b';
            else if (S_ISFIFO(st.st_mode)) type = 'p';
            else if (S_ISSOCK(st.st_mode)) type = 's';

            printf("%c", type);
            print_permissions(st.st_mode);

            // Кількість жорстких посилань
            printf(" %2lu", st.st_nlink);

            // Власник та група
            struct passwd *pw = getpwuid(st.st_uid);
            struct group  *gr = getgrgid(st.st_gid);
            printf(" %s %s", pw ? pw->pw_name : "?", gr ? gr->gr_name : "?");

            // Розмір
            printf(" %6ld", st.st_size);

            // Дата та час останньої модифікації
            char timebuf[64];
            strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M", localtime(&st.st_mtime));
            printf(" %s", timebuf);

            // Ім’я файлу
            printf(" %s\n", entry->d_name);
        } else {
            // Короткий формат — тільки ім’я
            printf("%s  ", entry->d_name);
        }
    }

    if (!show_long) printf("\n");

    closedir(dir);
    return 0;
}

