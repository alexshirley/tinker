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

// required_size calculates the necessary size to ensure is allocated for a cvector of a given size
unsigned long cvector_required_size(unsigned long element_size, unsigned long number_elements);

// init populates a cvector handle with a block, block size, and the size of each element. The entirety of the block size is used
// returns false if unsuccessful
bool cvector_init(cvector_t** __restrict vec, char* __restrict block, unsigned long block_size, unsigned long element_size);

// resize populates a cvector handle after a successfull realloc (or giving a different block size, if using automatic char[])
// returns false if unsucessful
bool cvector_resize(cvector_t** __restrict vec, char* __restrict block, unsigned long block_size);

// push_back pushes a value to the back of the cvector
// returns false if unsuccessful
bool cvector_push_back(cvector_t* __restrict vec, const void* __restrict value);

// pop_back removes a value from the back of the vector, writing it to the value provided
// returns false if unsuccessful
bool cvector_pop_back(cvector_t* __restrict vec, void* __restrict value);

// get takes an index and populates the value
// returns false if unsuccess
// value will be NULL if the element_index is invalid
bool cvector_get(const cvector_t* __restrict vec, unsigned long element_index, void* value);

// set takes an index and populates the cvector with the given value
// returns false if unsuccess
// value will be NULL if the element_index is invalid
bool cvector_set(cvector_t* __restrict vec, unsigned long element_index, const void* value);

// is_emptry tests if a vector is empty
// true for is_empty
// false for !is_empty
bool cvector_is_empty(const cvector_t* __restrict vec);

// capacity finds the maximum number of elements that can be put into the cvector
int cvector_capacity(const cvector_t* __restrict vec);

// reserve sets aside space in the cvector, as if it was populated with zeros
// returns false if unsuccessful (reserving more space than available, etc.)
bool cvector_reserve(cvector_t* __restrict vec, size_t number_of_elements);

// get_ref is the equivalent of the [] operator
// returns NULL if the index is out of range (negative, element missing, too big, etc.)
void* cvector_get_ref(const cvector_t* __restrict vec, unsigned long element_index);

// element_size returns the element size set during intiailization
size_t cvector_element_size(const cvector_t* __restrict v);

// size returns the number of elements currently stored
size_t cvector_size(const cvector_t* __restrict v);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINKE_CVECTOR_H Guard