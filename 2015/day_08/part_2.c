#include <string.h>
#include <stdio.h>

#include "load_file.h"

typedef unsigned int uint;

void read_line(char *line, uint *total_raw, uint *total_char);

int main () {
	char *input = load_file("2015/day_08/input.txt");
	char *line = strtok(input, "\n");

	uint total_raw = 0;
	uint total_char = 0;

	while (line != NULL) {
		// 2 "
		total_raw += 2;
		read_line(line, &total_raw, &total_char);

		line = strtok(NULL, "\n");
	}

	printf("%d - %d = %d\n", total_raw, total_char, total_raw - total_char);

	free(input);
	return 0;
}

void read_line(char *line, uint *total_raw, uint *total_char) {
	uint length = strlen(line);

	for (uint i = 0; i < length; i++) {
		switch (line[i]) {

		case '\\':
		case '"':
			*total_raw += 2; // \"
			*total_char += 1; // "

			break;
		default:
			*total_raw += 1;
			*total_char += 1;
			break;
		}
	}
}
