#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i = 0;
	char buff[104];
	i = 0;

	fgets(buff, 100, stdin);
	i = 0;

	while (strlen(buff) < i)
	{
		if (*(buff + i) > 64 /* @ <=> >= A */ && *(buff + i) <= 'Z')
		{
			// transform to lowercase
			*(buff + i) = *(buff + i) ^ 32;
		}
		i++;
	}

	printf(buff);
	exit(0);
}