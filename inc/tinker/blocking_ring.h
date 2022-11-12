#ifndef TINKER_NON_BLOCKING_RING_H
#define TINKER_NON_BLOCKING_RING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct blocking_ring blocking_ring_t;

size_t blocking_ring_required_size(unsigned long element_size, unsigned long number_elements);
bool blocking_ring_init(blocking_ring_t** __restrict c, char* block, size_t block_size, size_t element_size);
size_t blocking_ring_count(blocking_ring_t* __restrict c);
bool blocking_ring_pop(blocking_ring_t* __restrict c, void* __restrict pData);
bool blocking_ring_empty(blocking_ring_t* __restrict c);
bool blocking_ring_push(blocking_ring_t* __restrict c, const void* __restrict pData);
void blocking_ring_clear(blocking_ring_t* __restrict Fifo);
bool blocking_ring_full(blocking_ring_t* __restrict c);
size_t blocking_ring_capacity(blocking_ring_t* __restrict c);
void * blocking_ring_index(blocking_ring_t* __restrict c, uint32_t index);

#ifdef __cplusplus
} // extern C
#endif

#endif //guard
