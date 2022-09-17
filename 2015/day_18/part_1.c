#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "load_file.h"

#define GRID_LEN 100
#define GRID_SIZE 10000
#define ITERATION_LEN 100

const int offsets[8][2] = {
	{-1, -1}, {0, -1}, {1, -1},
	{-1,  0},          {1,  0},
	{-1,  1}, {0,  1}, {1,  1},
};

void mutate_grid_item(bool[GRID_SIZE], bool[GRID_SIZE], int);
void debug_grid(bool[GRID_SIZE]);

int main() {
	char *input = load_file("2015/day_18/input.txt");
	int input_len = strlen(input);

	// initialize grid;
	bool grid[GRID_SIZE];
	bool grid2[GRID_SIZE];
	int grid_i = 0;

	for (int i = 0; i < GRID_SIZE; i++) {
		grid[i] = false;
		grid2[i] = false;
	}

	for (int i = 0; i < input_len; i++) {

		switch (input[i]) {

		case '#':
			grid[grid_i] = true;
			grid_i += 1;
			break;
		case '.':
			grid[grid_i] = false;
			grid_i += 1;
			break;
		}
	}

	for (int i = 0; i < ITERATION_LEN; i++) {
		for (int j = 0; j < GRID_SIZE; j++) {
			mutate_grid_item(grid, grid2, j);
		}

		// copy
		for (int j = 0; j < GRID_SIZE; j++) {
			grid[j] = grid2[j];
		}
	}

	int active_len = 0;
	for (int i = 0; i < GRID_SIZE; i++) {
		if (grid[i]) active_len += 1;
	}

	printf("total: %d\n", active_len);

	free(input);
	return EXIT_SUCCESS;
}

void debug_grid(bool grid[GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; i++) {
		if (i != 0 && i % GRID_LEN == 0) printf("\n");
		printf(grid[i] ? "#" : ".");
	}
	printf("\n");
}

void mutate_grid_item(bool grid[GRID_SIZE], bool grid2[GRID_SIZE], int i) {
	bool *a = grid  + i;
	bool *b = grid2 + i;

	int an_len = 0;
	int pos[2] = {i % GRID_LEN, i / GRID_LEN};

	for (int j = 0; j < 8; j++) {
		int offset[2] = {offsets[j][0], offsets[j][1]};
		int x = offset[0] + pos[0];
		int y = offset[1] + pos[1];

		if (x < 0 || x >= GRID_LEN) continue;
		if (y < 0 || y >= GRID_LEN) continue;

		if (!grid[y * GRID_LEN + x]) continue;

		an_len += 1;
	}

	*b = *a ? an_len == 2 || an_len == 3 : an_len == 3;
}
