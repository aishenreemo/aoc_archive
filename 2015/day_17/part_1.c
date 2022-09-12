#include <stdlib.h>
#include <string.h>

#include "load_file.h"
#include "vector.h"

int main() {
	char *input = load_file("2015/day_17/input.txt");

	vector_t num_vec = vector_init();

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
	uint upper_bound = 1 << num_vec.length;
	for (uint i = 0; i < upper_bound; i++) {
		uint t = i;
		uint s = 0;
		for (uint j = 0; j < num_vec.length; j++) {
			uint *num = (uint *) vector_get_f(&num_vec, j);
			if (t % 2 == 1) s += *num;
			t /= 2;
		}

		if (s == 150) total += 1;
	}

	printf("total: %d\n", total);

	vector_drop_f(&num_vec);
	free(input);
	return 0;
}
