#include <stdio.h>
# define FOUND_SYSTEM 0b1
# define FOUND_BIN_SH 0b10

int main(void) {
	unsigned int max = -1;
	unsigned int default_val = (1 << 29); // 4 time less than max
	unsigned int n;
	char found = 0;

	for (int i = default_val; i < max && found != 3; i++) {
		n = i << 2;

		if (n == 456) {
			printf("%u - %u\n", i, i * 4);
			found |= FOUND_SYSTEM;
		}

		if (n == 464) {
			printf("%u - %u\n", i, i * 4);
			found |= FOUND_BIN_SH;
		}

		if ((i + 1) % 3 == 0)
			i++;
	}
}