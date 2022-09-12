#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "load_file.h"
#include "vector.h"

int main() {
	char *input = load_file("2015/day_17/input.txt");

	vector_t num_vec = vector_init();
	vector_t s_vec = vector_init();

	uint input_len = strlen(input);
	uint tmp_start = 0;

	for (uint i = 0; i < input_len; i++) {
		if (input[i] != '\n') continue;

		uint *num = malloc(sizeof(uint));
		sscanf(input + tmp_start, "%d", num);
		tmp_start = i + 1;
		vector_push_f(&num_vec, num);
	}

	uint total = 0;
	uint minimum = UINT_MAX;
	uint upper_bound = 1 << num_vec.length;
	for (uint i = 0; i < upper_bound; i++) {
		uint t = i;
		uint s = 0;
		uint c = 0;

		for (uint j = 0; j < num_vec.length; j++) {
			uint *num = (uint *) vector_get_f(&num_vec, j);
			bool is_odd = t % 2 == 1;
			if (is_odd) s += *num;
			if (is_odd) c += 1;
			t /= 2;
		}

		if (s == 150) minimum = minimum < c ? minimum : c;
	}

	for (uint i = 0; i < upper_bound; i++) {
		uint t = i;
		uint s = 0;
		uint c = 0;

		for (uint j = 0; j < num_vec.length; j++) {
			uint *num = (uint *) vector_get_f(&num_vec, j);
			bool is_odd = t % 2 == 1;
			if (is_odd) s += *num;
			if (is_odd) c += 1;
			t /= 2;
		}

		if (s == 150 && c == minimum) total += 1;
	}

	printf("total: %d\n", total);

	vector_drop_f(&num_vec);
	vector_drop_f(&s_vec);
	free(input);
	return 0;
}
