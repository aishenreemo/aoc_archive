#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "load_file.h"

// defines the current location of santa
typedef struct {
	int x;
	int y;
} location_t;

int is_already_visited(location_t house, location_t *houses, int total);
void mutate_loc(char ch, location_t *loc);

int main() {
	char *input = load_file("2015/day_03/input.txt");

	int length = strlen(input);
	int total = 1;

	location_t *houses = calloc(sizeof(location_t), total);
	location_t santa_loc;
	location_t robot_loc;

	houses[0].x = 0;
	houses[0].y = 0;

	santa_loc = houses[0];
	robot_loc = houses[0];

	char *santa_ptr = input + 0;
	char *robot_ptr = input + 1;

	while (santa_ptr != input + length && robot_ptr != input + length) {
		mutate_loc(*santa_ptr, &santa_loc);
		mutate_loc(*robot_ptr, &robot_loc);

		if (!is_already_visited(santa_loc, houses, total)) {
			houses = realloc(houses, sizeof(location_t) * (total + 1));
			houses[total] = santa_loc;
			total += 1;
		}

		if (!is_already_visited(robot_loc, houses, total)) {
			houses = realloc(houses, sizeof(location_t) * (total + 1));
			houses[total] = robot_loc;
			total += 1;
		}

		santa_ptr += 2;
		robot_ptr += 2;
	}

	printf("Santa delivered presents to %d houses.\n", total);

	free(input);
	free(houses);
	return 0;
}

int is_already_visited(location_t house, location_t *houses, int total) {
	for (int i = 0; i < total; i++) {
		if (house.x == houses[i].x && house.y == houses[i].y) {
			return 1;
		}
	}

	return 0;
}

void mutate_loc(char ch, location_t *loc) {
	switch (ch) {

	case '>':
		(*loc).x += 1;
		break;
	case '<':
		(*loc).x -= 1;
		break;
	case '^':
		(*loc).y += 1;
		break;
	case 'v':
		(*loc).y -= 1;
		break;
	}
}
