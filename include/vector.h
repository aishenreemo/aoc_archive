#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

enum vector_result_e {
	VEC_RESULT_SUCCESS,
	VEC_RESULT_UNDEFINED,
	VEC_RESULT_OUT_OF_BOUNDS,
};

struct vector_s {
	void **items;
	size_t length;
};

enum vector_result_e vector_remove_f (struct vector_s *, int);
enum vector_result_e vector_push_f (struct vector_s *, void *);
void vector_drop_f (struct vector_s *);
void *vector_get_f (struct vector_s *, int);

struct vector_s vector_init();

typedef enum vector_result_e vector_result_t;
typedef struct vector_s vector_t;
typedef struct vector_ops_s vector_ops_t;

#endif // VECTOR_H
