#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_file.h"

int contains_char_pair_twice(char *str);
int contains_char_that_repeats_between_char(char *str);

int main() {
	char *input = load_file("2015/day_05/input.txt");
	char *line = strtok(input, "\n");

	unsigned int total = 0;

	while (line != NULL) {
		int is_nice_string = contains_char_pair_twice(line) &&
			contains_char_that_repeats_between_char(line);

		if (is_nice_string) total += 1;

		printf("%s -> %s\n", line, is_nice_string ? "nice" : "naughty");

		line = strtok(NULL, "\n");
	}

	printf("%d nice strings.\n", total);

	free(input);
	return 0;
}

int contains_char_pair_twice(char *str) {
	int length = strlen(str);

	int i = 0;
	while (i + 1 < length) {
		char *ch1 = str + i + 0;
		char *ch2 = str + i + 1;

		int j = 0;

		while(j + 1 < length) {
			char *ch3 = str + j + 0;
			char *ch4 = str + j + 1;
			int diff = abs(i - j);

			j += 1;

			if (diff < 2) continue;
			if (*ch1 == *ch3 && *ch2 == *ch4) return 1;
		}

		i += 1;
	}

	return 0;
}

int contains_char_that_repeats_between_char(char *str) {
	int length = strlen(str);

	for (int i = 0; (i + 2) < length; i++) {
		if (*(str + i + 0) == *(str + i + 2)) return 1;
	}

	return 0;
}
