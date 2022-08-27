#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>

#include "load_file.h"
#include "vector.h"

typedef enum {
	EXPR_UNKNOWN,
	EXPR_LSHIFT,
	EXPR_RSHIFT,
	EXPR_NOT,
	EXPR_AND,
	EXPR_OR,
} expression_t;

typedef struct {
	vector_t args;
	expression_t expr;
} statement_t;

typedef struct {
	char *ptr;
	size_t len;
} slice_t;

typedef struct {
	slice_t key;
	uint16_t value;
} data_t;

void debug_statements(vector_t *statements);
void into_statements(vector_t *statements, char *input);
void drop_statements(vector_t *statements);
void get_expr_type(slice_t *arg, expression_t *expr);
void debug_memory(vector_t *memory);

void run(vector_t *statements, vector_t *memory);

int main() {
	char *input = load_file("2015/day_07/input.txt");
	vector_t statements = vector_init();
	vector_t memory = vector_init();

	into_statements(&statements, input);
	run(&statements, &memory);

	data_t b;
	for (size_t i = 0; i < memory.length; i++) {
		data_t *d = (data_t *) vector_get_f(&memory, i);
		if (strncmp("a", d->key.ptr, 1) != 0 || d->key.len != 1) continue;

		slice_t key;
		key.ptr = "b";
		key.len = 1;

		b.key = key;
		b.value = d->value;
	}

	vector_drop_f(&memory);
	memory = vector_init();
	vector_push_f(&memory, &b);

	run(&statements, &memory);

	debug_statements(&statements);
	debug_memory(&memory);

	drop_statements(&statements);
	vector_drop_f(&memory);
	free(input);
	return 0;
}

void into_statements(vector_t *statements, char *input) {
	const size_t input_len = strlen(input);

	size_t tmp_arg_start = 0;
	size_t tmp_arg_end = 0;

	expression_t expr = EXPR_UNKNOWN;
	vector_t args = vector_init();

	for (size_t i = 0; i < input_len; i++) {
		char *c = input + i;

		switch (*c) {

		case ' ': {
			slice_t *arg = calloc(sizeof(slice_t *), 1);
			arg->ptr = input + tmp_arg_start;
			arg->len = tmp_arg_end - tmp_arg_start;

			if (expr == EXPR_UNKNOWN) get_expr_type(arg, &expr);

			vector_push_f(&args, arg);
			tmp_arg_start = tmp_arg_end + 1;
			break;
		}

		case '\n': {
			if (args.length == 0) break;
			slice_t *arg = calloc(sizeof(slice_t *), 1);
			statement_t *statement = calloc(sizeof(statement_t *), 1);

			arg->ptr = input + tmp_arg_start;
			arg->len = tmp_arg_end - tmp_arg_start;

			vector_push_f(&args, arg);
			tmp_arg_start = tmp_arg_end + 1;

			statement->args = args;
			statement->expr = expr;

			vector_push_f(statements, statement);

			args = vector_init();
			expr = EXPR_UNKNOWN;
			break;
		}}

		tmp_arg_end += 1;
	}
}

void drop_statements(vector_t *statements) {
	for (size_t i = 0; i < statements->length; i++) {
		statement_t *s = (statement_t *) vector_get_f(statements, i);
		vector_drop_f(&s->args);
	}

	vector_drop_f(statements);
}

void debug_statements(vector_t *statements) {
	if (statements == NULL) return;

	printf("[\n");
	for (size_t i = 0; i < statements->length; i++) {
		statement_t *s = (statement_t *) vector_get_f(statements, i);

		if (s == NULL) break;

		printf("\t{ ");
		printf("args: [");
		for (size_t j = 0; j < s->args.length; j++) {
			slice_t *a = (slice_t *) vector_get_f(&s->args, j);
			printf("\"%.*s\"", (int) a->len, a->ptr);

			if ((j + 1) < s->args.length) printf(", ");
		}
		printf("], ");

		switch (s->expr) {

		case EXPR_LSHIFT:
			printf("operator: lshift");
			break;
		case EXPR_RSHIFT:
			printf("operator: rshift");
			break;
		case EXPR_NOT:
			printf("operator: not");
			break;
		case EXPR_AND:
			printf("operator: and");
			break;
		case EXPR_OR:
			printf("operator: or");
			break;
		default:
			printf("operator: unknown");
			break;
		}


		printf(" }");
		if ((i + 1) < statements->length) printf(",");
		printf("\n");
	}
	printf("]\n");
	return;
}

void debug_memory(vector_t *memory) {
	for (size_t i = 0; i < memory->length; i++) {
		data_t *d = (data_t *) vector_get_f(memory, i);
		printf("%.*s: %d\n", (int) d->key.len, d->key.ptr, d->value);
	}
}

void get_expr_type(slice_t *arg, expression_t *expr) {
	const char *ops[] = { "LSHIFT", "RSHIFT", "NOT", "AND", "OR" };
	const char enum_ops[5] = { EXPR_LSHIFT, EXPR_RSHIFT, EXPR_NOT, EXPR_AND, EXPR_OR };

	for (int i = 0; i < 5; i++) {
		if (strncmp(ops[i], arg->ptr, strlen(ops[i])) == 0) {
			*expr = enum_ops[i];
			break;
		}
	}
}

data_t *get_data(vector_t *memory, slice_t *key) {
	// linear search
	for (size_t i = 0; i < memory->length; i++) {
		data_t *m = (data_t *) vector_get_f(memory, i);

		if (
			strncmp(key->ptr, m->key.ptr, key->len) == 0 &&
			key->len == m->key.len
		) return m;
	}

	return NULL;
}

int into_u16(vector_t *memory, slice_t *ident, uint16_t *val) {
	if (isdigit(*ident->ptr)) {
		char *p;
		*val = strtoumax(ident->ptr, &p, 10);
		return 0;
	}

	data_t *d = get_data(memory, ident);
	if (d == NULL) return 1;

	*val = d->value;
	return 0;
}


void set_data(vector_t *memory, slice_t *key, uint16_t val) {
	data_t *d = get_data(memory, key);

	if (d != NULL) return;

	d = calloc(sizeof(data_t), 1);
	d->key = *key;
	d->value = val;
	vector_push_f(memory, d);
}

void run(vector_t *statements, vector_t *memory) {
	#define GET_ARG(i) vector_get_f(&s->args, i)

	for (
		size_t i = 0;
		memory->length < statements->length;
		i = (i + 1) % statements->length
	) {
		statement_t *s = (statement_t *) vector_get_f(statements, i);

		uint16_t v1;
		uint16_t v2;
		uint16_t out;


		// output identifier aka last argument of the statement
		slice_t *oi = (slice_t *) GET_ARG(s->args.length - 1);

		switch (s->expr) {

		case EXPR_LSHIFT:
			if (into_u16(memory, GET_ARG(0), &v1) == 1) continue;
			    into_u16(memory, GET_ARG(2), &v2);

			out = v1 << v2;
			break;
		case EXPR_RSHIFT:
			if (into_u16(memory, GET_ARG(0), &v1) == 1) continue;
			    into_u16(memory, GET_ARG(2), &v2);

			out = v1 >> v2;
			break;
		case EXPR_NOT:
			if (into_u16(memory, GET_ARG(1), &v1) == 1) continue;

			out = ~v1;
			break;
		case EXPR_AND:
			if (into_u16(memory, GET_ARG(0), &v1) == 1) continue;
			if (into_u16(memory, GET_ARG(2), &v2) == 1) continue;

			out = v1 & v2;
			break;
		case EXPR_OR:
			if (into_u16(memory, GET_ARG(0), &v1) == 1) continue;
			if (into_u16(memory, GET_ARG(2), &v2) == 1) continue;

			out = v1 | v2;
			break;
		case EXPR_UNKNOWN:
			if (into_u16(memory, GET_ARG(0), &v1) == 1) continue;

			out = v1;
			break;
		}

		set_data(memory, oi, out);
	}
}

