#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "load_file.h"

typedef struct {
	char *ptr;
	uint length;
} password_t;

void incr(password_t *pass, uint place);
bool is_valid(password_t *pass);

int main(void) {
	char *input = load_file("2015/day_11/input.txt");

	// initialize password
	password_t pass;
	pass.length = strlen(input) - 1;
	pass.ptr = calloc(sizeof(char), pass.length + 1);
	strcpy(pass.ptr, strtok(input, "\n"));

	while (!is_valid(&pass)) incr(&pass, 0);

	printf("%s\n", pass.ptr);

	// dealloc
	free(pass.ptr);
	free(input);
	return 0;
}

void incr(password_t *pass, uint place) {
	// if the unit place overlap
	if (place < 0 || place >= pass->length) return;

	// increment the password
	char *ch = &pass->ptr[pass->length - (place + 1)];
	*ch += 1;

	// if char is still in the alphabet
	if (*ch <= 'z') return;

	// if not, make char into 'a' then carry one
	*ch = 'a';
	place += 1;

	// prepend a char if not existing yet
	if (place == pass->length) {
		pass->length += 1;
		pass->ptr = realloc(pass->ptr, sizeof(char) * (pass->length + 1));
		for (uint i = pass->length - 1; i - 1 >= 0; i--) {
			pass->ptr[i] = pass->ptr[i - 1];
		}

		pass->ptr[0] = 'a';
	}

	// carry one
	incr(pass, place);
}

bool is_valid(password_t *pass) {
	bool has_consecutive_chars = false;
	bool has_excluding_chars = false;
	bool has_two_pairs = false;

	// Passwords must include one increasing straight
	// of at least three letters, like abc, bcd, cde,
	// and so on, up to xyz. They cannot skip letters;
	for (uint i = 0; i + 3 < pass->length; i++) {
		if (
			pass->ptr[i + 2] != pass->ptr[i + 1] + 1 ||
			pass->ptr[i + 2] != pass->ptr[i + 0] + 2
		) continue;

		has_consecutive_chars = true;
		break;
	}

	// Passwords may not contain the letters i, o, or l,
	// as these letters can be mistaken for other
	// characters and are therefore confusing.
	for (uint i = 0; i < pass->length; i++) {
		if (
			pass->ptr[i] != 'i' ||
			pass->ptr[i] != 'o' ||
			pass->ptr[i] != 'l'
		) continue;

		has_excluding_chars = true;
		break;
	}

	// Passwords must contain at least two different,
	// non-overlapping pairs of letters, like aa, bb, or zz.
	char *p1 = NULL;
	for (uint i = 0; i + 1 < pass->length; i++) {
		char *p = pass->ptr + i;

		if (p[0] != p[1]) continue;

		if (p1 == NULL) {
			p1 = p;
			continue;
		}

		if (strncmp(p1, p, 2) == 0) continue;

		has_two_pairs = true;
		break;
	}

	return has_consecutive_chars && !has_excluding_chars && has_two_pairs;
}
