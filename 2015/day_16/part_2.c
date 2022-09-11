#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "load_file.h"

enum compound_e {
	COMP_CHILDREN,
	COMP_CATS,
	COMP_SAMOYEDS,
	COMP_POMERANIANS,
	COMP_AKITAS,
	COMP_VIZSLAS,
	COMP_GOLDFISH,
	COMP_TREES,
	COMP_CARS,
	COMP_PERFUMES,
};

typedef enum compound_e compound_t;
compound_t into_compound(char[20]);
bool check(compound_t, uint);

#define COMPOUNDS_LEN COMP_PERFUMES + 1

int main() {
	char *input = load_file("2015/day_16/input.txt");

	uint input_len = strlen(input);
	uint tmp_start = 0;

	for (uint i = 0, line = 0; i < input_len; i++) {
		if (input[i] != '\n') continue;

		uint *a = calloc(sizeof(uint), COMPOUNDS_LEN);
		for (uint i = COMP_CHILDREN; i <= COMP_PERFUMES; i++) a[i] = 0;

		char properties[3][20];
		uint values[3];
		uint _index;

		sscanf(
			input + tmp_start,
			"Sue %d: %[^:]: %d, %[^:]: %d, %[^:]: %d\n",
			&_index,
			properties[0], &values[0],
			properties[1], &values[1],
			properties[2], &values[2]
		);

		uint score = 0;
		for (uint j = 0; j < 3; j++) {
			compound_t comp = into_compound(properties[j]);
			uint value = values[j];
			if (check(comp, value)) score += 1;
		}

		line += 1;
		tmp_start = i + 1;

		if (score != 3) continue;
		printf("%d\n", line);
	}

	free(input);
	return 0;
}

bool check(compound_t comp, uint value) {
	switch (comp) {

	case COMP_CHILDREN:
		return value == 3;
	case COMP_CATS:
		return value > 7;
	case COMP_SAMOYEDS:
		return value == 2;
	case COMP_POMERANIANS:
		return value < 3;
	case COMP_AKITAS:
		return value == 0;
	case COMP_VIZSLAS:
		return value == 0;
	case COMP_GOLDFISH:
		return value < 5;
	case COMP_TREES:
		return value > 3;
	case COMP_CARS:
		return value == 2;
	case COMP_PERFUMES:
		return value == 1;
	}
}

compound_t into_compound(char *key) {
	if (strcmp(key, "children") == 0) {
		return COMP_CHILDREN;
	} else if (strcmp(key, "cats") == 0) {
		return COMP_CATS;
	} else if (strcmp(key, "samoyeds") == 0) {
		return COMP_SAMOYEDS;
	} else if (strcmp(key, "pomeranians") == 0) {
		return COMP_POMERANIANS;
	} else if (strcmp(key, "akitas") == 0) {
		return COMP_AKITAS;
	} else if (strcmp(key, "vizslas") == 0) {
		return COMP_VIZSLAS;
	} else if (strcmp(key, "goldfish") == 0) {
		return COMP_GOLDFISH;
	} else if (strcmp(key, "trees") == 0) {
		return COMP_TREES;
	} else if (strcmp(key, "cars") == 0) {
		return COMP_CARS;
	} else if (strcmp(key, "perfumes") == 0) {
		return COMP_PERFUMES;
	} else {
		return COMP_CHILDREN;
	}
}
