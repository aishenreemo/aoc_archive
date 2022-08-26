#include <inttypes.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "../../include/load_file.h"

typedef enum {
	EXPR_UNKNOWN,
	EXPR_LSHIFT,
	EXPR_RSHIFT,
	EXPR_NOT,
	EXPR_AND,
	EXPR_OR,
} expression_type_t;

typedef struct {
	char *ptr;
	size_t len;
} str_slice_t;

typedef struct {
	size_t args_len;
	str_slice_t *args;
	expression_type_t expr;
} statement_t;

typedef struct {
	str_slice_t *key;
	uint16_t value;
} data_t;

statement_t *into_statements(char *input, size_t *statements_len);

data_t *get_data(data_t *memory, size_t *mem_len, str_slice_t *key);
void set_data(data_t *memory, size_t *mem_len, str_slice_t *key, uint16_t val);

int into_u16(data_t *memory, size_t *mem_len, str_slice_t *ident, uint16_t *val);

void get_expr_type(str_slice_t *arg, expression_type_t *expr);
void drop_statements(statement_t *statements, size_t statements_len);
void debug_statements(statement_t *statements, size_t statements_len);
void compute_solution(statement_t *statements, size_t statements_len);

int main() {
	char *input = load_file("2015/day_07/input.txt");

	size_t statements_len = 0;
	statement_t *statements = into_statements(input, &statements_len);

	// debug_statements(statements, statements_len);

	compute_solution(statements, statements_len);

	drop_statements(statements, statements_len);

	free(input);
	return 0;
}

void compute_solution(statement_t *statements, size_t statements_len) {
	size_t mem_len = 0;
	data_t memory[statements_len];

	for (size_t i = 0; mem_len < statements_len; i = (i + 1) % statements_len) {
		statement_t *s = statements + i;

		uint16_t v1;
		uint16_t v2;
		uint16_t out;

		str_slice_t *out_ident = s->args + (s->args_len - 1);

		switch (s->expr) {

		case EXPR_LSHIFT:
			if (into_u16(memory, &mem_len, s->args + 0, &v1) == 1) continue;
			into_u16(memory, &mem_len, s->args + 2, &v2);

			out = v1 << v2;
			break;
		case EXPR_RSHIFT:
			if (into_u16(memory, &mem_len, s->args + 0, &v1) == 1) continue;
			into_u16(memory, &mem_len, s->args + 2, &v2);

			out = v1 >> v2;
			break;
		case EXPR_NOT:
			if (into_u16(memory, &mem_len, s->args + 1, &v1) == 1) continue;

			out = ~v1;
			break;
		case EXPR_AND:
			if (into_u16(memory, &mem_len, s->args + 0, &v1) == 1) continue;
			if (into_u16(memory, &mem_len, s->args + 2, &v2) == 1) continue;

			out = v1 & v2;
			break;
		case EXPR_OR:
			if (into_u16(memory, &mem_len, s->args + 0, &v1) == 1) continue;
			if (into_u16(memory, &mem_len, s->args + 2, &v2) == 1) continue;

			out = v1 | v2;
			break;
		case EXPR_UNKNOWN:
			if (into_u16(memory, &mem_len, s->args + 0, &v1) == 1) continue;

			out = v1;
			break;
		}

		set_data(memory, &mem_len, out_ident, out);
	}

	for (size_t i = 0; i < mem_len; i++) {
		data_t *d = memory + i;
		printf("%.*s: %d\n", (int) d->key->len, d->key->ptr, d->value);
	}
}


int into_u16(data_t *memory, size_t *mem_len, str_slice_t *ident, uint16_t *val) {
	if (isdigit(*ident->ptr)) {
		char *p;
		*val = strtoumax(ident->ptr, &p, 10);
		return 0;
	}

	data_t *d = get_data(memory, mem_len, ident);
	if (d == NULL) return 1;

	*val = d->value;
	return 0;
}

data_t *get_data(data_t *memory, size_t *mem_len, str_slice_t *key) {
	// linear search
	for (size_t i = 0; i < *mem_len; i++) {
		data_t *m = memory + i;

		if (m->key == NULL) continue;

		if (strncmp(key->ptr, m->key->ptr, key->len) == 0 && (m->key->len == key->len)) {
			return m;
		}
	}

	return NULL;
}

void set_data(data_t *memory, size_t *mem_len, str_slice_t *key, uint16_t val) {
	data_t *d = get_data(memory, mem_len, key);

	if (d != NULL) {
		d->value = val;
		return;
	}

	memory[*mem_len].key = key;
	memory[*mem_len].value = val;
	*mem_len += 1;
}

void debug_statements(statement_t *statements, size_t statements_len) {
	for (size_t i = 0; i < statements_len; i++) {
		statement_t *s = statements + i;
		printf("statement %i:\n", (int) i);

		switch (s->expr) {

		case EXPR_LSHIFT:
			printf("\toperator: lshift\n");
			break;
		case EXPR_RSHIFT:
			printf("\toperator: rshift\n");
			break;
		case EXPR_NOT:
			printf("\toperator: not\n");
			break;
		case EXPR_AND:
			printf("\toperator: and\n");
			break;
		case EXPR_OR:
			printf("\toperator: or\n");
			break;
		case EXPR_UNKNOWN:
			printf("\toperator: unknown\n");
			break;
		}

		printf("\targument length: %i\n", (int) s->args_len);
		printf("\targs: [ ");
		for (size_t j = 0; j < s->args_len; j++) {
			printf("\"%.*s\" ", (int) s->args[j].len, s->args[j].ptr);
		}
		printf("]\n");
	}

}

void drop_statements(statement_t *statements, size_t statements_len) {
	for (size_t i = 0; i < statements_len; i++) {
		statement_t *s = statements + i;
		free(s->args);
	}

	free(statements);
}

void get_expr_type(str_slice_t *arg, expression_type_t *expr) {
	char *ops[] = { "LSHIFT", "RSHIFT", "NOT", "AND", "OR" };
	char enum_ops[5] = { EXPR_LSHIFT, EXPR_RSHIFT, EXPR_NOT, EXPR_AND, EXPR_OR };

	for (int i = 0; i < 5; i++) {
		if (strncmp(ops[i], arg->ptr, arg->len) == 0) {
			*expr = enum_ops[i];
			break;
		}
	}
}

statement_t *into_statements(char *input, size_t *statements_len) {
	// length of the input
	size_t input_len = strlen(input);
	// length of words for each line
	size_t args_len = 0;

	// struct that represents the line
	statement_t *statements = calloc(sizeof(statement_t), 1);
	// line separated by spaces
	str_slice_t *args = calloc(sizeof(str_slice_t), 1);

	// what operator is used in the line
	expression_type_t expr = EXPR_UNKNOWN;

	// cursor to the first letter of that arg
	int tmp_arg_start = 0;
	// cursor to the last letter(+1) of that args
	int tmp_arg_end = 0;

	// for each character in input
	for (size_t i = 0; i < input_len; i++) {
		char *ch = input + i;

		str_slice_t arg;
		statement_t statement;

		switch (*ch) {

		// move to new arg
		case ' ':;
			// push argument
			arg.ptr = input + tmp_arg_start;
			arg.len = tmp_arg_end - tmp_arg_start;

			// if the operator of this statement is not
			// determined yet
			if (expr == EXPR_UNKNOWN) get_expr_type(&arg, &expr);

			args[args_len] = arg;
			args_len += 1;
			args = realloc(args, sizeof(str_slice_t) * (args_len + 1));

			// go to new argument
			tmp_arg_start = tmp_arg_end + 1;

			break;

		// move to new line
		case '\n':;
			// push argument
			arg.ptr = input + tmp_arg_start;
			arg.len = tmp_arg_end - tmp_arg_start;

			args[args_len] = arg;
			args_len += 1;

			// go to new argument
			tmp_arg_start = tmp_arg_end + 1;

			// push statement
			statement.args_len = args_len;
			statement.args = args;
			statement.expr = expr;

			statements[*statements_len] = statement;
			*statements_len += 1;
			statements = realloc(statements, sizeof(statement_t) * (*statements_len + 1));

			// reset
			args_len = 0;
			args = calloc(sizeof(str_slice_t), 1);
			expr = EXPR_UNKNOWN;
			break;
		}

		tmp_arg_end += 1;

	}

	return statements;
}
