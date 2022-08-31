#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "load_file.h"

typedef unsigned int uint;
char *look_and_say(char *input);

int main(void) {
	char *input = load_file("2015/day_10/input.txt");

	for (uint i = 0; i < 50; i++) {
		char *out = look_and_say(input);
		uint out_len = strlen(out);
		input = realloc(input, sizeof(char) * (out_len) + 1);
		strcpy(input, out);
		free(out);
	}

	printf("%s\n%ld\n", input, strlen(input));

	free(input);
	return 0;
}

char *look_and_say(char *input) {
	uint out_len = 0;
	uint str_len = strlen(input);

	char *out = (char *) calloc(sizeof(char), out_len + 3);

	out[out_len + 0] = '\0';

	for (uint i = 0; i < str_len && isdigit(input[i]); i++) {
		char ch = input[i];

		uint count = 1; for (; (i + 1) < str_len && input[i + 1] == ch; i++, count++);

		char count_ch = count + '0';

		out[out_len + 0] = count_ch;
		out[out_len + 1] = ch;
		out[out_len + 2] = '\0';

		out_len += 2;
		out = (char *) realloc(out, sizeof(char) * (out_len + 3));
	}

	return out;
}
