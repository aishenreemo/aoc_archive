#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "load_file.h"

unsigned int surface_area(unsigned int l, unsigned int w, unsigned int h);
unsigned int smallest_side_area(unsigned int l, unsigned int w, unsigned int h);

int main() {
	char *input = load_file("2015/day_02/input.txt");

	// first line
	char *line = strtok(input, "\n");

	unsigned int total = 0;

	// iterate for each line in the input
	while (line != NULL) {
		unsigned int l; // length
		unsigned int w; // width
		unsigned int h; // height

		// parse line
		if (sscanf(line, "%ux%ux%u", &l, &w, &h) != 3) {
			printf("Failed to parse input.\n%s\n", line);
			goto exit_l;
		}

		// calculate surface_area
		unsigned int sa = surface_area(l, w, h);
		// calculate the area of the smallest side.
		unsigned int ssa = smallest_side_area(l, w, h);

		total += sa;
		total += ssa;

		line = strtok(NULL, "\n");
	}

	printf("Elves should order %d square feet of wrapping paper.\n", total);

	exit_l:
		free(input);
		return 0;
}

// each present is a right rectangular prism
unsigned int surface_area(unsigned int l, unsigned int w, unsigned int h) {
	return (2 * l * w) + (2 * w * h) + (2 * h * l);
}

unsigned int smallest_side_area(unsigned int l, unsigned int w, unsigned int h) {
	unsigned int arr[3] = { l * w, w * h, h * l };
	unsigned int smallest = arr[0];

	for (int i = 0; i < 3; i++) {
		if (arr[i] > smallest) continue;
		smallest = arr[i];
	}

	return smallest;
}
