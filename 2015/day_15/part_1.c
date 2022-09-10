#include <string.h>
#include <stdio.h>

#include "load_file.h"
#include "vector.h"

struct ingredient_s {
	char name[15];
	int capacity;
	int durability;
	int flavor;
	int texture;
	int calories;
};

typedef struct ingredient_s ingredient_t;

int main() {
	char *input = load_file("2015/day_15/input.txt");

	vector_t ingredient_vec = vector_init();
	vector_t nums_vec = vector_init();

	uint input_len = strlen(input);
	uint tmp_start = 0;

	// transform input into a vector of ingredients
	for (uint i = 0; i < input_len; i++) {
		if (input[i] != '\n') continue;

		ingredient_t *ingr = malloc(sizeof(ingredient_t));

		sscanf(
			input + tmp_start,
			"%[^:]: capacity %i, durability %i, flavor %i, texture %i, calories %i",
			ingr->name,
			&ingr->capacity,
			&ingr->durability,
			&ingr->flavor,
			&ingr->texture,
			&ingr->calories
		);

		tmp_start = i + 1;
		vector_push_f(&ingredient_vec, ingr);
	}

	// determine all combination of N whole numbers that sums up to exact 100
	// each whole number must be greater than 0 and less than 100 (0 < N < 100)
	// N = length of ingredients
	uint upper_bound = 1;
	uint mods[ingredient_vec.length];
	for (uint i = 0; i < ingredient_vec.length; i++) {
		upper_bound *= 100;
		mods[i] = 1; for (uint j = 0; j < i * 2; j++) mods[i] *= 10;
	}

	for (uint i = 0; i < upper_bound; i++) {
		uint sum = 0;
		int tmp[ingredient_vec.length];
		for (uint j = 0; j < ingredient_vec.length; j++) {
			tmp[j] = (i / mods[j]) % 100 + 1;
			sum += tmp[j];
		}

		if (sum != 100) continue;
		int *comb = calloc(sizeof(int), ingredient_vec.length);
		for (uint j = 0; j < ingredient_vec.length; j++) comb[j] = tmp[j];
		vector_push_f(&nums_vec, comb);
	}

	// determine the highest-scoring cookie you can make
	uint highest_total = 0;
	for (uint i = 0; i < nums_vec.length; i++) {
		int *comb = (int *) vector_get_f(&nums_vec, i);

		int total_capacity = 0;
		int total_durability = 0;
		int total_flavor = 0;
		int total_texture = 0;
		int _total_calories = 0;

		for (uint j = 0; j < ingredient_vec.length; j++) {
			ingredient_t *ingr = (ingredient_t *) vector_get_f(&ingredient_vec, j);

			total_capacity += ingr->capacity * comb[j];
			total_durability += ingr->durability * comb[j];
			total_flavor += ingr->flavor * comb[j];
			total_texture += ingr->texture * comb[j];
			_total_calories += ingr->calories * comb[j];
		}

		total_capacity = total_capacity > 0 ? total_capacity : 0;
		total_durability = total_durability > 0 ? total_durability : 0;
		total_flavor = total_flavor > 0 ? total_flavor : 0;
		total_texture = total_texture > 0 ? total_texture : 0;
		_total_calories = _total_calories > 0 ? _total_calories : 0;

		int total = total_capacity * total_durability * total_flavor * total_texture;

		highest_total = highest_total > total ? highest_total : total;
	}

	printf("highest: %d\n", highest_total);

	vector_drop_f(&ingredient_vec);
	vector_drop_f(&nums_vec);
	free(input);
	return 0;
}
