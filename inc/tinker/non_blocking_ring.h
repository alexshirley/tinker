#ifndef TINKER_NON_BLOCKING_RING_H
#define TINKER_NON_BLOCKING_RING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ring non_blocking_ring_t;

unsigned long non_blocking_ring_required_size(unsigned long element_size, unsigned long number_elements);
bool non_blocking_ring_init(non_blocking_ring_t** __restrict c, char* block, size_t block_size, size_t element_size);
uint32_t non_blocking_ring_count(non_blocking_ring_t* __restrict c);
bool non_blocking_ring_pop(non_blocking_ring_t* __restrict c, void* __restrict pData);
bool non_blocking_ring_empty(non_blocking_ring_t* __restrict c);
bool non_blocking_ring_push(non_blocking_ring_t* __restrict c, const void* __restrict pData);
void non_blocking_ring_clear(non_blocking_ring_t* __restrict Fifo);
bool non_blocking_ring_full(non_blocking_ring_t* __restrict c);
uint32_t non_blocking_ring_capacity(non_blocking_ring_t* __restrict c);
void * non_blocking_ring_index(non_blocking_ring_t* __restrict c, uint32_t index);

#ifdef __cplusplus
} // extern C
#endif

#endif //guard
