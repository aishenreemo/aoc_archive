#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "load_file.h"

int main() {
	char *input = load_file("2015/day_12/input.txt");
	uint input_len = strlen(input);

	int total = 0;

	for (uint i = 0; i < input_len; i++) {
		if (!isdigit(input[i]) && input[i] != '-') continue;

		bool is_negative = false;
		if (input[i] == '-') {
			is_negative = true;
			i += 1;
		}

		int num = 0;
		for (; i < input_len && isdigit(input[i]); num = num * 10 + (input[i] - '0'), i++);

		total += is_negative ? -num : num;
	}

	printf("%d\n", total);

	free(input);
	return 0;
}
