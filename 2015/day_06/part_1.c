#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/load_file.h"

#define GRID_MAX	999
#define GRID_POS(X, Y)	Y * GRID_MAX + X

#define ACTION_ON	1
#define ACTION_OFF	2
#define ACTION_TOGGLE	3

void mutate_grid(int *grid, int action_type, int position);

int main() {
	char *input = load_file("2015/day_06/input.txt");
	char *line = strtok(input, "\n");

	int *grid = calloc(GRID_MAX * GRID_MAX * sizeof(int), 1);

	while (line != NULL) {
		int x1, y1;
		int x2, y2;

		char action_str[9];
		int action_type;

		sscanf(
			line,
			"%[^0123456789] %i,%i through %i,%i\n",
			action_str,
			&x1, &y1,
			&x2, &y2
		);

		if (strncmp("toggle", action_str, 6) == 0) {
			action_type = ACTION_TOGGLE;
		} else if (strncmp("turn on", action_str, 7) == 0) {
			action_type = ACTION_ON;
		} else if (strncmp("turn off", action_str, 8) == 0) {
			action_type = ACTION_OFF;
		}

		int diff_x = (x2 + 1) - x1;
		int diff_y = (y2 + 1) - y1;

		for (int i = 0; i < (diff_x * diff_y); i++) {
			int col = (i % diff_x) + x1;
			int row = (i / diff_x) + y1;

			mutate_grid(grid, action_type, GRID_POS(col, row));
		}

		line = strtok(NULL, "\n");
	}

	int total = 0;
	for (int i = 0; i < (GRID_MAX * GRID_MAX); i++) {
		if (grid[i]) total += 1;
	}

	printf("%d lights are lit.\n", total);

	free(input);
	free(grid);

	return 0;
}

void mutate_grid(int *grid, int action_type, int position) {
	switch (action_type) {

	case ACTION_OFF:
		grid[position] = 0;
		break;
	case ACTION_ON:
		grid[position] = 1;
		break;
	case ACTION_TOGGLE:
		grid[position] = grid[position] ? 0 : 1;
		break;
	}

}
