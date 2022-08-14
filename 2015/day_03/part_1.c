#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/load_file.h"

// defines the current location of santa
typedef struct {
	int x;
	int y;
} location_t;

int is_already_visited(location_t house, location_t *houses, int total);

int main() {
	char *input = load_file("2015/day_03/input.txt");

	int length = strlen(input);
	int total = 1;

	location_t *houses = calloc(sizeof(location_t), total);
	location_t current_location = {
		.x = 0,
		.y = 0
	};

	houses[0] = current_location;

	for (char *c = input; c != input + length; c += 1) {

		switch (*c) {

		case '>':
			current_location.x += 1;
			break;
		case '<':
			current_location.x -= 1;
			break;
		case '^':
			current_location.y += 1;
			break;
		case 'v':
			current_location.y -= 1;
			break;
		}

		// if santa already visited this house, just continue to next loop
		if (is_already_visited(current_location, houses, total)) continue;

		// resize vector then push current location
		houses = realloc(houses, sizeof(location_t) * (total + 1));
		houses[total] = current_location;
		total += 1;
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
