#include <stdio.h>
#include <stdlib.h>

int main() {
    int input;

    puts("***********************************");
    puts("* 	     -Level00 -		        *");
    puts("***********************************");
    printf("Password:");
    scanf("%d", &input);

    if (input == 5276) {
        puts("Authenticated!");
        system("/bin/sh");
        return 0;
    } else {
        puts("Invalid Password!");
        return 1;
    }
}
