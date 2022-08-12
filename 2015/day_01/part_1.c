#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/load_file.h"

int main() {
	char *input = load_file("2015/day_01/input.txt");

	int length = strlen(input);
	int floor_index = 0;

	for (char *c = input; c != input + length; c += 1) {
		switch (*c) {

		case '(':
			floor_index += 1;
			break;
		case ')':
			floor_index -= 1;
			break;
		}
	}

	printf("Santa is at floor no. %d\n", floor_index);

	free(input);
	return 0;
}
