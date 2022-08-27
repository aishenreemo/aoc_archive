#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

enum vector_result_e vector_remove_f (struct vector_s *vec, int i) {
	if (vec == NULL) return VEC_RESULT_UNDEFINED;
	if (i < 0 || i >= vec->length) return VEC_RESULT_OUT_OF_BOUNDS;

	void **items = realloc(vec->items, sizeof(void *) * vec->length);
	if (items == NULL) return VEC_RESULT_UNDEFINED;

	for (; (i + 1) < vec->length; i++) items[i] = items[i + 1];

	vec->length -= 1;
	free(items[vec->length]);
	items[vec->length] = NULL;
	vec->items = items;

	return VEC_RESULT_SUCCESS;
}

enum vector_result_e vector_push_f (struct vector_s *vec, void *item) {
	if (vec == NULL) return VEC_RESULT_UNDEFINED;
	void **items = realloc(vec->items, sizeof(void *) * (vec->length + 1));
	if (items == NULL) return VEC_RESULT_UNDEFINED;

	items[vec->length] = item;
	vec->length += 1;
	vec->items = items;

	return VEC_RESULT_SUCCESS;
}

void vector_drop_f (struct vector_s *vec) {
	vec->length = 0;
	for (size_t i = 0; i < vec->length; i++) free(vec->items[i]);
	free(vec->items);
}

void *vector_get_f (struct vector_s *vec, int i) {
	if (vec == NULL) return NULL;
	if (i < 0 || i >= vec->length) return NULL;

	return vec->items[i];
}

struct vector_s vector_init() {
	struct vector_s vec;

	vec.items = calloc(sizeof(void *), 1);
	vec.length = 0;

	return vec;
}
