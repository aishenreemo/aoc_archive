#include <stdlib.h>
#include <string.h>

#include "load_file.h"
#include "vector.h"

struct reindeer_s {
	char name[8];
	uint velocity;
	uint work_time;
	uint rest_time;
};

typedef struct reindeer_s reindeer_t;

uint calculate_distance(reindeer_t *);

const uint GAME_TIME = 2503;

int main() {
	// transform input into a vector of reindeer struct
	vector_t reindeer_vec = vector_init();

	char *input = load_file("2015/day_14/input.txt");

	uint input_len = strlen(input);
	uint tmp_start = 0;

	for (uint i = 0; i < input_len; i++) {
		if (input[i] != '\n') continue;

		reindeer_t *r = malloc(sizeof(reindeer_t));

		sscanf(
			input + tmp_start,
			"%s can fly %d km/s for %d seconds, but then must rest for %d seconds.",
			r->name,
			&r->velocity,
			&r->work_time,
			&r->rest_time
		);

		tmp_start = i + 1;
		vector_push_f(&reindeer_vec, r);
	}

	// for each reindeer calculate the distance travelled
	uint longest = 0;
	for (uint i = 0; i < reindeer_vec.length; i++) {
		reindeer_t *r = (reindeer_t *) vector_get_f(&reindeer_vec, i);
		uint distance = calculate_distance(r);
		printf(
			"%-10s %4d %4d %4d -> %7d\n",
			r->name,
			r->velocity,
			r->work_time,
			r->rest_time,
			distance
		);

		longest = longest > distance ? longest : distance;
	}

	printf("longest: %d\n", longest);

	free(input);
	return 0;
}

uint calculate_distance(reindeer_t *r) {
	uint rep = GAME_TIME / (r->work_time + r->rest_time);
	uint mod = GAME_TIME % (r->work_time + r->rest_time);
	uint min = r->work_time < mod ? r->work_time : mod;

	return (rep * r->work_time * r->velocity) + (min * r->velocity);
}
