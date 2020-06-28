#ifndef TINKER_PRIO_Q_H
#define TINKER_PRIO_Q_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <cvector.h>

typedef struct prio_q prio_q_t;

// Comparison function is of the same type as qsort
// Making the comparison negative will make the first item go up the tree
// Making the comparison positive will make the second item go up the tree
typedef int (*compare)(const void*, const void*);

size_t prio_q_required_size(unsigned long element_size, unsigned long number_elements);
bool prio_q_init(prio_q_t** p, char* block, unsigned long block_size, unsigned long element_size, compare func);
void prio_q_create_from_cvec(prio_q_t** p, char block[16], cvector_t* vec, compare func);
bool prio_q_peek(prio_q_t* p, void* out);
bool prio_q_push(prio_q_t* q, const void* value);
bool prio_q_pop(prio_q_t* q, void* value);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINKER_PRIO_Q_H Guard