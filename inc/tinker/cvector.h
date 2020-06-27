#ifndef TINKER_CVECTOR_H
#define TINKER_CVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif
/*
cvector is a rough replacment for std::vector in C++, supporting basic functions such as accessing by index, push_back, pop_back
*/

#include <stdbool.h>
#include <stddef.h>

typedef struct cvector cvector_t;

unsigned long cvector_required_size(unsigned long element_size, unsigned long number_elements);
bool cvector_init(cvector_t** vec, char* block, unsigned long block_size, unsigned long element_size);
bool cvector_resize(cvector_t** vec, char* block, unsigned long block_size);
bool cvector_push_back(cvector_t* vec, const void* value);
bool cvector_pop_back(cvector_t* vec, void* value);
bool cvector_get(cvector_t* vec, unsigned long element_index, void* value);
bool cvector_set(cvector_t* vec, unsigned long element_index, void* value);
bool cvector_is_empty(cvector_t* vec);
int cvector_capacity(cvector_t* vec);
bool cvector_reserve(cvector_t* vec, size_t number_of_elements);
void* cvector_get_ref(cvector_t* vec, unsigned long element_index);
size_t cvector_element_size(cvector_t* v);
size_t cvector_size(cvector_t* v);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINKE_CVECTOR_H Guard