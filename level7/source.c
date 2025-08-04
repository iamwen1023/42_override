#include <stdio.h>

void clear_stdin(void)
{
	char c;

	do
	{
		c = getchar();
	} while (c != '\n' && c != EOF);
}

unsigned int get_unum(void)
{
	unsigned int input; // esp+28

	fflush(stdout);
	scanf("%u", &input);
	clear_stdin();
	return input;
}

int store_number(char *ptr_store)
{
	unsigned int input_index = 0; // esp+28
	unsigned int input_nbr = 0;	  // esp+24

	printf(" Number: ");
	input_nbr = get_unum();
	printf(" Index: ");
	input_index = get_unum();

	if (input_index % 3 == 0 || input_nbr >> 24 == 183)
	{
		puts(" *** ERROR! ***");
		puts("   This index is reserved for wil!");
		puts(" *** ERROR! ***");
		return (1);
	}
	*(ptr_store + (input_index * 4)) = input_nbr;
	return (0);
}

int read_number(char *ptr_store)
{
	unsigned int input_index = 0; // esp+28

	printf(" Index: ");
	input_index = get_unum();

	printf(" Number at data[%u] is %u\n", input_index, *((input_index * 4) + ptr_store));
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	char input[24];			 // esp+440
	int action_result;		 // esp+436
	char ptr_store[100];	 // esp+36
	char **argv_copy = argv; // esp+28
	char **envp_copy = envp; // esp+24

	bzero(input, 24);
	bzero(ptr_store, 100);
	while (*argv_copy)
	{
		bzero(*argv_copy, strlen(*argv_copy) - 1);
		argv_copy++;
	}
	while (*envp_copy)
	{
		bzero(*envp_copy, strlen(*envp_copy) - 1);
		envp_copy++;
	}
	puts("----------------------------------------------------\n\
  Welcome to wil's crappy number storage service!\n\
----------------------------------------------------\n\
 Commands:\n\
    store - store a number into the data storage\n\
    read  - read a number from the data storage\n\
    quit  - exit the program\n\
----------------------------------------------------\n\
   wil has reserved some storage :>\n\
----------------------------------------------------\n");

	while (1)
	{
		printf("Input command: ");
		action_result = 1;
		fgets(input, 20, stdin);
		*(input + strlen(input) - 2) = 0;

		if (!memcmp(input, "store", 5))
		{
			action_result = store_number(ptr_store);
		}
		else if (!memcmp(input, "read", 4))
		{
			action_result = read_number(ptr_store);
		}
		else if (!memcmp(input, "quit", 4))
		{
			break;
		}

		if (action_result == 0)
		{
			printf(" Completed %s command successfully\n", input);
		}
		else
		{
			printf(" Failed to do %s command\n", input);
		}
		bzero(input, 16);
	}
	return (0);
}