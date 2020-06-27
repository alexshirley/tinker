#ifndef TINKER_PRIO_Q_H
#define TINKER_PRIO_Q_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>


typedef struct prio_q prio_q_t;
typedef int (*compare)(const void *, const void *);

size_t prio_q_required_size(unsigned long element_size, unsigned long number_elements);
bool prio_q_init(prio_q_t** p, char* block, unsigned long size, unsigned long element_size, compare func);
bool prio_q_peek(prio_q_t * p, void* out);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // TINKER_PRIO_Q_H Guard