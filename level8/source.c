#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void log_wrapper(FILE *file_ptr, char *message, char *file_name)
{
    char str_copy[272];

    strcpy(str_copy, message);
    // The file name is used for format. So maybe format string attack.
    snprintf(str_copy + strlen(str_copy) - 1, 255 - strlen(str_copy), file_name); // str, size, format
    *strchr(str_copy, '\n') = 0;
    fprintf(file_ptr, "LOG: %s\n", str_copy);
}

int main(int argc, char **argv)
{
    char letter[1] = {255};

    if (argc != 2)
    {
        printf("Usage: %s filename\n", *argv);
    }

    FILE *log = fopen("./backups/.log", "w");
    if (!log)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    log_wrapper(log, "Starting back up: ", argv[1]);

    FILE *read_file = fopen(argv[1], "r"); // rbp-0x80
    if (!read_file)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    char path[64]; // rbp-0x70
    strcpy(path, "./backups/");
    strncat(path, argv[1], 100 - strlen(path));

    int fd = open(path, 193, 432); // rbp-0x78
    if (fd < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    while ((*letter = fgetc(read_file)) != EOF)
    {
        write(fd, letter, 1);
    }

    log_wrapper(log, "Finished back up ", argv[1]);
    fclose(read_file);
    close(fd);
    return 0;
}