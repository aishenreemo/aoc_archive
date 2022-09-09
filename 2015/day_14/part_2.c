#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "load_file.h"
#include "vector.h"

struct reindeer_s {
	char name[8];
	uint velocity;
	uint work_time;
	uint rest_time;
	uint distance;
	uint points;
};

typedef struct reindeer_s reindeer_t;

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

		r->points = 0;
		r->distance = 0;

		tmp_start = i + 1;
		vector_push_f(&reindeer_vec, r);
	}

	// simulate the race for each second
	for (uint s = 0; s < GAME_TIME; s++) {
		// initialize leaderboard
		uint score = 0;

		// for each reindeer, accumulate velocity to distance if its not resting
		// also determine the distance of leading reindeer
		for (uint i = 0; i < reindeer_vec.length; i++) {
			reindeer_t *r = (reindeer_t *) vector_get_f(&reindeer_vec, i);
			uint mod = s % (r->work_time + r->rest_time);
			if (mod < r->work_time) r->distance += r->velocity;
			if (score > r->distance) continue;
			score = r->distance;
		}

		// for each reindeer, accumulate 1 point if it's score is the highest
		for (uint i = 0; i < reindeer_vec.length; i++) {
			reindeer_t *r = (reindeer_t *) vector_get_f(&reindeer_vec, i);
			if (score != r->distance) continue;
			r->points += 1;
		}
	}

	uint highest = 0;
	for (uint i = 0; i < reindeer_vec.length; i++) {
		reindeer_t *r = (reindeer_t *) vector_get_f(&reindeer_vec, i);
		printf(
			"%-10s %4dkm/s %4ds of worktime %4ds of restime -> %4dkm (%4dpts)\n",
			r->name,
			r->velocity,
			r->work_time,
			r->rest_time,
			r->distance,
			r->points
		);

		highest = highest > r->points ? highest : r->points;
	}

	printf("highest: %d\n", highest);
	vector_drop_f(&reindeer_vec);
	free(input);
	return 0;
}
