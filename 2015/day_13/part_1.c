#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "load_file.h"
#include "vector.h"

enum state_e {
	STATE_GAIN,
	STATE_LOSE,
};

struct line_s {
	char from[20];
	char to[20];
	enum state_e state;
	int happiness;
};

struct relation_s {
	char key[40];
	int value;
};

typedef enum state_e state_t;
typedef struct line_s line_t;
typedef struct relation_s relation_t;

vector_t get_lines(char *);
vector_t get_persons(vector_t *);
vector_t get_relations(vector_t *);

bool insert_person(vector_t *, char *);
bool insert_relation(vector_t *, relation_t *);

void solve(int, int, int *, vector_t *, vector_t *);

int main() {
	char *input = load_file("2015/day_13/input.txt");

	vector_t line_vec = get_lines(input);
	vector_t person_vec = get_persons(&line_vec);
	vector_t relation_vec = get_relations(&line_vec);

	for (uint i = 0; i < relation_vec.length; i++) {
		relation_t *r = (relation_t *) vector_get_f(&relation_vec, i);
		printf("%-15s %5d\n", r->key, r->value);
	}

	int greatest = INT_MIN;
	solve(0, person_vec.length - 1, &greatest, &person_vec, &relation_vec);
	printf("happiness total: %d\n", greatest);

	vector_drop_f(&line_vec);
	vector_drop_f(&person_vec);
	free(input);
	return 0;
}

void swap(vector_t *person_vec, int a, int b) {
	char temp[20];
	char *p1 = (char *) vector_get_f(person_vec, a);
	char *p2 = (char *) vector_get_f(person_vec, b);

	strcpy(temp, p1);
	strcpy(p1, p2);
	strcpy(p2, temp);
}

int calculate_total_happiness(vector_t *person_vec, vector_t *relation_vec) {
	int total = 0;

	char key[40];
	char *p1;
	char *p2;

	for (uint i = 0; i + 1 < person_vec->length; i++) {
		p1 = (char *) vector_get_f(person_vec, i + 0);
		p2 = (char *) vector_get_f(person_vec, i + 1);

		snprintf(key, 40, "%s-%s", p1, p2);

		for (uint j = 0; j < relation_vec->length; j++) {
			relation_t *r = (relation_t *) vector_get_f(relation_vec, j);
			if (strcmp(r->key, key) != 0) continue;
			total += r->value;
			break;
		}
	}

	p1 = (char *) vector_get_f(person_vec, 0);
	p2 = (char *) vector_get_f(person_vec, person_vec->length - 1);

	snprintf(key, 40, "%s-%s", p1, p2);

	for (uint i = 0; i < relation_vec->length; i++) {
		relation_t *r = (relation_t *) vector_get_f(relation_vec, i);
		if (strcmp(r->key, key) != 0) continue;
		total += r->value;
		break;
	}

	return total;
}

void solve(
	int start,
	int length,
	int *greatest,
	vector_t *person_vec,
	vector_t *relation_vec
) {
	if (start == length) {
		int tmp_distance = calculate_total_happiness(person_vec, relation_vec);
		*greatest = (*greatest > tmp_distance) ? *greatest : tmp_distance;
		return;
	}


	for (int i = start; i <= length; i++) {
		swap(person_vec, start, i);
		solve(start + 1, length, greatest, person_vec, relation_vec);
		swap(person_vec, start, i);
	}
}

vector_t get_lines(char *input) {
	vector_t out = vector_init();

	uint input_len = strlen(input);
	uint tmp_start = 0;

	for (uint i = 0; i < input_len; i++) {
		if (input[i] != '\n') continue;

		char state[5];
		line_t *l = malloc(sizeof(line_t));

		sscanf(
			input + tmp_start,
			"%s would %s %d happiness units by sitting next to %[^.].\n",
			l->from,
			state,
			&l->happiness,
			l->to
		);

		l->state = strcmp(state, "gain") == 0 ? STATE_GAIN : STATE_LOSE;
		tmp_start = i + 1;

		vector_push_f(&out, l);
	}

	return out;
}

vector_t get_persons(vector_t *line_vec) {
	vector_t out = vector_init();

	for (uint i = 0; i < line_vec->length; i++) {
		line_t *l = (line_t *) vector_get_f(line_vec, i);

		char *p1 = calloc(sizeof(char), 20);
		char *p2 = calloc(sizeof(char), 20);

		strcpy(p1, l->from);
		strcpy(p2, l->to);

		if (!insert_person(&out, p1)) free(p1);
		if (!insert_person(&out, p2)) free(p2);
	}

	return out;
}

vector_t get_relations(vector_t *line_vec) {
	vector_t out = vector_init();

	for (uint i = 0; i < line_vec->length; i++) {
		char tmp_key[40];

		relation_t *r1 = malloc(sizeof(relation_t));
		relation_t *r2 = malloc(sizeof(relation_t));
		line_t *l1 = (line_t *) vector_get_f(line_vec, i);

		snprintf(r1->key, 40, "%s-%s", l1->from, l1->to);
		snprintf(r2->key, 40, "%s-%s", l1->to, l1->from);

		int value = 0;

		for (uint j = 0; j < line_vec->length; j++) {
			line_t *l2 = (line_t *) vector_get_f(line_vec, j);

			snprintf(tmp_key, 40, "%s-%s", l2->to, l2->from);

			if (strcmp(tmp_key, r1->key) != 0) continue;

			int value1 = l1->state == STATE_GAIN ? l1->happiness : -(l1->happiness);
			int value2 = l2->state == STATE_GAIN ? l2->happiness : -(l2->happiness);

			value = value1 + value2;
			printf("%d %d\n", value1, value2);
			break;
		}

		r1->value = value;
		r2->value = value;

		if (!insert_relation(&out, r1)) free(r1);
		if (!insert_relation(&out, r2)) free(r2);
	}

	return out;
}

bool insert_person(vector_t *person_vec, char *person) {
	for (uint i = 0; i < person_vec->length; i++) {
		char *p = (char *) vector_get_f(person_vec, i);

		if (strcmp(p, person) == 0) return false;
	}

	vector_push_f(person_vec, person);
	return true;
}

bool insert_relation(vector_t *relation_vec, relation_t *relation) {
	for (uint i = 0; i < relation_vec->length; i++) {
		char *p = (char *) vector_get_f(relation_vec, i);

		if (strcmp(p, relation->key) == 0) return false;
	}

	vector_push_f(relation_vec, relation);
	return true;
}
