#include <stdio.h>
#include <string.h>

int auth(char *buff, int serial)
{
    *strchr(buff, '\n') = 0;
    size_t size = strnlen(buff, 32); // ebp-0xc <=> esp+28

    if (size > 5)
    {
        if (ptrace(0, 0, 1, 0) == -1)
        {
            puts("\x1b[32m.---------------------------.");
            puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
            puts("\x1b[32m.---------------------------.");
            return (1);
        }

        int local_char = (int)(buff[3] ^ 4919) + 0x5eeded; // ebp-0x10
        int i = 0;                                         // ebp-0x14
        while (i < size)
        {
            int old_c = buff[i];
            if (old_c <= 31)
                return (1);

            local_char += (old_c ^ local_char) % 1337;
            ++i;
        }

        if (serial == local_char)
            return (0);
    }
    return (1);
}

int main(int argc, char **argv)
{
    char buff[16];          // esp + 0x44 -> à partir de 12 octets on overflow
    int serial;             // esp+0x28
    char **argv_cpy = argv; // esp+0x1c

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");

    fgets(buff, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    __isoc99_scanf("%u", serial);

    if (!auth(buff, serial))
    {
        puts("Authenticated!");
        system("/bin/sh");
        return (0);
    }
    return (1);
}