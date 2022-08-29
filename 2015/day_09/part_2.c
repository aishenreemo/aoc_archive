#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#include "load_file.h"

#define PLACES_CAPACITY 10
#define PLACE_STR_CAPACITY 20
#define DISTANCES_CAPACITY 60

typedef unsigned int uint;

typedef struct {
	char source_to_dest[PLACE_STR_CAPACITY];
	uint distance;
} dict;

bool insert_dict(char source_to_dest[PLACE_STR_CAPACITY], uint distance);
bool insert(char place[PLACE_STR_CAPACITY]);
void solve(uint start, uint length, uint *longest);
void swap(char (*ptr)[20], char (*ptr2)[20]);
uint calculate_distance();

char places[PLACES_CAPACITY][PLACE_STR_CAPACITY];
dict distances[DISTANCES_CAPACITY];

int main() {
	// initialize all declared strings as empty
	for (int i = 0; i < PLACES_CAPACITY; i++) places[i][0] = '\0';
	for (int i = 0; i < DISTANCES_CAPACITY; i++) distances[i].source_to_dest[0] = '\0';

	// parse input into data
	char *input = load_file("2015/day_09/input.txt");
	char *line = strtok(input, "\n");
	while (line != NULL) {
		char c1[PLACE_STR_CAPACITY];
		char c2[PLACE_STR_CAPACITY];
		char c1toc2[PLACE_STR_CAPACITY];
		char c2toc1[PLACE_STR_CAPACITY];
		uint val;

		sscanf(line, "%s to %s = %i\n", c1, c2, &val);
		snprintf(c1toc2, PLACE_STR_CAPACITY, "%s>%s", c1, c2);
		snprintf(c2toc1, PLACE_STR_CAPACITY, "%s>%s", c2, c1);

		insert(c1);
		insert(c2);

		insert_dict(c1toc2, val);
		insert_dict(c2toc1, val);

		line = strtok(NULL, "\n");
	}

	// permutate places
	// get the length of the set
	uint places_len = 0; while (places[places_len][0] != '\0') places_len += 1;
	uint longest = 0;

	solve(0, places_len - 1, &longest);
	printf("longest %d\n", longest);

	free(input);
	return 0;
}

uint calculate_distance() {
	uint total = 0;
	for (int i = 0; places[i + 1][0] != '\0'; i++) {
		char source_to_dest[PLACE_STR_CAPACITY];
		snprintf(
			source_to_dest,
			PLACE_STR_CAPACITY,
			"%s>%s",
			places[i + 0],
			places[i + 1]
		);

		for (int j = 0; distances[j].source_to_dest[0] != '\0'; j++) {
			if (strcmp(distances[j].source_to_dest, source_to_dest) != 0) continue;
			total += distances[j].distance;
		}
	}

	return (total == 0) ? UINT_MAX : total;
}

void swap(char (*ptr)[20], char (*ptr2)[20]) {
	char temp[20];
	strcpy(temp, *ptr);
	strcpy(*ptr, *ptr2);
	strcpy(*ptr2, temp);
}

void solve(uint start, uint length, uint *longest) {

	if (start == length) {
		uint tmp_distance = calculate_distance();
		*longest = (*longest > tmp_distance) ? *longest : tmp_distance;
		return;
	}

	for (int i = start; i <= length; i++) {
		swap((places + start), (places + i));
		solve(start + 1, length, longest);
		swap((places + start), (places + i));
	}
}

bool insert(char place[PLACE_STR_CAPACITY]) {
	int i;
	for (i = 0; places[i][0] != '\0'; i++) {
		if (strcmp(places[i], place) == 0) return false;
	}

	strcpy(places[i], place);

	return true;
}

bool insert_dict(char source_to_dest[PLACE_STR_CAPACITY], uint distance) {
	int i;
	for (i = 0; distances[i].source_to_dest[0] != '\0'; i++) {
		if (strcmp(distances[i].source_to_dest, source_to_dest) == 0) return false;
	}

	strcpy(distances[i].source_to_dest, source_to_dest);
	distances[i].distance = distance;

	return true;
}
