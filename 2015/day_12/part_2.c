#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "load_file.h"
#include "vector.h"

typedef struct {
	char *ptr;
	uint length;
} slice_t;

typedef enum {
	TOKEN_UNKNOWN,
	TOKEN_ARRAY_DELIMITER_OPEN, // [
	TOKEN_ARRAY_DELIMITER_CLOSE, // ]
	TOKEN_OBJECT_DELIMITER_OPEN, // {
	TOKEN_OBJECT_DELIMITER_CLOSE, // }
	TOKEN_STRING, // "string"
	TOKEN_NUMBER, // 123
	TOKEN_COMMA, // ,
	TOKEN_COLON, // :
} token_type_t;

typedef struct {
	slice_t str;
	token_type_t variant;
	bool ignore;
} token_t;

vector_t tokenize(char *);
int compute_solution(vector_t *);

void debug_tokens(vector_t *);
void ignore_red_objs(vector_t *);
void ignore_obj(vector_t *tokens, uint i);

int main(void) {
	char *input = load_file("2015/day_12/input.txt");
	vector_t tokens = tokenize(input);

	// debug_tokens(&tokens);
	ignore_red_objs(&tokens);
	printf("%d\n", compute_solution(&tokens));

	vector_drop_f(&tokens);
	free(input);
	return 0;
}

void ignore_red_objs(vector_t *tokens) {
	// ignore objects with "red" value
	for (uint i = 0; i + 1 < tokens->length; i++) {
		token_t *t0 = (token_t *) vector_get_f(tokens, i + 0);
		token_t *t1 = (token_t *) vector_get_f(tokens, i + 1);

		if (t0->variant != TOKEN_COLON) continue;
		if (t1->variant != TOKEN_STRING) continue;

		if (t1->str.length != 5) continue;
		if (t1->str.ptr[1] != 'r') continue;
		if (t1->str.ptr[2] != 'e') continue;
		if (t1->str.ptr[3] != 'd') continue;

		ignore_obj(tokens, i);
	}
}

#define GET(i) (token_t *) vector_get_f(tokens, i)
void ignore_obj(vector_t *tokens, uint index) {
	// determine the position of opening delimiter ({)
	// and closing delimiter (}) in the object
	// where token[i] is placed

	uint left_index = index;
	uint right_index = index;

	// {
	for (uint stack = 1; stack > 0; left_index--) {
		token_t *token = GET(left_index);
		if (token == NULL) {
			printf("index error: tokens[%d] is null\n", left_index);
			return;
		}

		switch (token->variant) {

		case TOKEN_OBJECT_DELIMITER_OPEN:
			stack -= 1;
			break;
		case TOKEN_OBJECT_DELIMITER_CLOSE:
			stack += 1;
			break;
		default:
			break;
		}
	}

	// }
	for (uint stack = 1; stack > 0; right_index++) {
		token_t *token = GET(right_index);

		if (token == NULL) {
			printf("index error: tokens[%d] is null\n", right_index);
			printf("stack: %d\n", stack);
			return;
		}

		switch (token->variant) {

		case TOKEN_OBJECT_DELIMITER_OPEN:
			stack += 1;
			break;
		case TOKEN_OBJECT_DELIMITER_CLOSE:
			stack -= 1;
			break;
		default:
			break;
		}
	}

	// mark tokens as empty
	for (uint i = left_index; i <= right_index; i++) {
		token_t *token = GET(i);
		token->ignore = true;
	}
}

int compute_solution(vector_t *tokens) {
	int total = 0;
	for (uint i = 0; i < tokens->length; i++) {
		token_t *token = GET(i);

		if (token->ignore) continue;
		if (token->variant != TOKEN_NUMBER) continue;

		uint j = 0;
		int num = 0;

		bool is_negative = token->str.ptr[0] == '-';
		if (is_negative) j += 1;

		while (j < token->str.length && isdigit(token->str.ptr[j])) {
			num = num * 10 + (token->str.ptr[j] - '0');
			j += 1;
		}

		total += is_negative ? -num : num;
	}

	return total;
}

void debug_tokens(vector_t *tokens) {
	for (uint i = 0; i < tokens->length; i++) {
		token_t *token = GET(i);
		switch (token->variant) {

		case TOKEN_UNKNOWN:
			printf("TOKEN_UNKNOWN: ");
			break;
		case TOKEN_ARRAY_DELIMITER_OPEN:
			printf("TOKEN_ARRAY_DELIMITER_OPEN: ");
			break;
		case TOKEN_ARRAY_DELIMITER_CLOSE:
			printf("TOKEN_ARRAY_DELIMITER_CLOSE: ");
			break;
		case TOKEN_OBJECT_DELIMITER_OPEN:
			printf("TOKEN_OBJECT_DELIMITER_OPEN: ");
			break;
		case TOKEN_OBJECT_DELIMITER_CLOSE:
			printf("TOKEN_OBJECT_DELIMITER_CLOSE: ");
			break;
		case TOKEN_COMMA:
			printf("TOKEN_COMMA: ");
			break;
		case TOKEN_COLON:
			printf("TOKEN_COLON: ");
			break;
		case TOKEN_STRING:
			printf("TOKEN_STRING: ");
			break;
		case TOKEN_NUMBER:
			printf("TOKEN_NUMBER: ");
			break;
		}

		printf("%.*s -> %d\n", token->str.length, token->str.ptr, token->str.length);
	}

}

vector_t tokenize(char *input) {
	vector_t tokens = vector_init();
	uint input_len = strlen(input);

	uint tmp_start = 0;

	for (uint i = 0; i < input_len; i++) {
		token_t *token = calloc(sizeof(token_t), 1);
		token->variant = TOKEN_UNKNOWN;
		token->str.ptr = input + tmp_start;
		token->str.length = (i + 1) - tmp_start;
		token->ignore = false;

		switch (input[i]) {

		case ',':
			token->variant = TOKEN_COMMA;
			break;
		case ':':
			token->variant = TOKEN_COLON;
			break;
		case '[':
			token->variant = TOKEN_ARRAY_DELIMITER_OPEN;
			break;
		case ']':
			token->variant = TOKEN_ARRAY_DELIMITER_CLOSE;
			break;
		case '{':
			token->variant = TOKEN_OBJECT_DELIMITER_OPEN;
			break;
		case '}':
			token->variant = TOKEN_OBJECT_DELIMITER_CLOSE;
			break;
		case '-':
		case '0' ... '9':
			if (input[i] == '-') i += 1;

			for (; i < input_len && isdigit(input[i]); i++);

			token->variant = TOKEN_NUMBER;
			token->str.ptr = input + tmp_start;
			token->str.length = i - tmp_start;

			i -= 1;
			break;
		case '"':
			i += 1;
			for (; i < input_len && input[i] != '"'; i++);
			i += 1;

			token->variant = TOKEN_STRING;
			token->str.ptr = input + tmp_start;
			token->str.length = i - tmp_start;

			i -= 1;
			break;
		default:
			break;
		}

		tmp_start = i + 1;
		vector_push_f(&tokens, token);
	}

	return tokens;
}
