#ifndef TINKER_RING_H
#define TINKER_RING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ring ring_t;

unsigned long ring_required_size(unsigned long element_size, unsigned long number_elements);
bool ring_init(ring_t** __restrict c, char* block, size_t block_size, size_t element_size);
uint32_t ring_count(ring_t* __restrict c);
bool ring_pop(ring_t* __restrict c, void* __restrict pData);
bool ring_empty(ring_t* __restrict c);
bool ring_push(ring_t* __restrict c, const void* __restrict pData);
void ring_clear(ring_t* __restrict Fifo);
bool ring_full(ring_t* __restrict c);

#ifdef __cplusplus
} // extern C
#endif

#endif //guard