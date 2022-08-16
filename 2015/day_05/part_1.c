#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/load_file.h"

int count_vowels(char *str);
int check_char_twice(char *str);
int check_naughty_strings(char *str);

int main() {
	char *input = load_file("2015/day_05/input.txt");
	char *line = strtok(input, "\n");

	unsigned int total = 0;

	while (line != NULL) {
		int contains_three_vowels = count_vowels(line) >= 3;
		int contains_char_twice = check_char_twice(line);
		int contains_naughty_strings = check_naughty_strings(line);

		int is_a_nice_string = contains_three_vowels &&
			contains_char_twice &&
			!contains_naughty_strings;

		if (is_a_nice_string) total += 1;

		line = strtok(NULL, "\n");
	}

	printf("%d nice strings.\n", total);

	free(input);
	return 0;
}

int count_vowels(char *str) {
	int count = 0;
	int length = strlen(str);

	for (char *ch = str; ch != str + length; ch += 1) {
		char ch_l = tolower(*ch);

		if (
			ch_l == 'a' ||
			ch_l == 'e' ||
			ch_l == 'i' ||
			ch_l == 'o' ||
			ch_l == 'u'
		) count += 1;
	}

	return count;
}

int check_char_twice(char *str) {
	int length = strlen(str);

	char *ch1 = str + 0;
	char *ch2 = str + 1;

	while (ch1 != str + length && ch2 != str + length) {
		if (*ch1 == *ch2) return 1;

		ch1 += 1;
		ch2 += 1;
	}

	return 0;
}

int check_naughty_strings(char *str) {
	int length = strlen(str);

	char *ch1 = str + 0;
	char *ch2 = str + 1;

	while (ch1 != str + length && ch2 != str + length) {
		if (
			(*ch1 == 'a' && *ch2 == 'b') ||
			(*ch1 == 'c' && *ch2 == 'd') ||
			(*ch1 == 'p' && *ch2 == 'q') ||
			(*ch1 == 'x' && *ch2 == 'y')
		) return 1;

		ch1 += 1;
		ch2 += 1;
	}

	return 0;
}
