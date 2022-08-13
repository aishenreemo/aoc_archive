#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/load_file.h"

int main() {
	char *input = load_file("2015/day_01/input.txt");

	int length = strlen(input);
	int floor_index = 0;

	for (int i = 0; i < length; i += 1) {
		char *c = input + i;

		switch (*c) {

		case '(':
			floor_index += 1;
			break;
		case ')':
			floor_index -= 1;
			break;
		}

		// if santa got on the basement
		if (floor_index < 0) {
			printf("Santa got on the basement for %d turns\n", i + 1);
			goto exit_l;
		}
	}

	printf("Santa never got on the basement.");

	exit_l:
		free(input);
		return 0;
}
