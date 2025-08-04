#include <string.h>
#include <stdio.h>

void secret_backdoor()
{
	char buff[128]; // rbp-0x80

	fgets(buff, 128, stdin);
	system(buff);
}

void set_msg(char *message)
{					 // rbp-1032
	char buff[1024]; // rbp-1024

	bzero(buff, 128);
	puts(">: Msg @Unix-Dude");
	printf(">>: ");
	fgets(buff, 1024, stdin);
	strncpy(message, buff, message[180]);
}

void set_username(char *username) // rbp - 152
{
	char buff[140]; // rbp - 144
	int res;		// rbp - 4
	bzero(buff, 16);

	puts(">: Enter your username");
	printf(">>: ");
	fgets(buff, 128, stdin);
	res = 0;

	while (res <= 40 && buff[res] != '\0')
	{
		*(username + 140 + res) = buff[res];
		res++;
	}
	printf(">: Welcome, %s", username + 140);
}

void handle_msg(void)
{
	char c;				// rbp-12
	char ptr[40];		// rbp-52
	char username[140]; // rbp-192

	bzero(ptr, 40);
	c = 140;
	set_username(username);
	set_msg(username);
	puts(">: Msg sent!");
}

int main(void)
{
	puts("--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------");
	handle_msg();
	return (0);
}