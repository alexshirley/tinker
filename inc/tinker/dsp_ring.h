#ifndef TINKER_DSP_RING_H
#define TINKER_DSP_RING_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dsp_ring dsp_ring_t;

unsigned long dsp_ring_required_size(unsigned long element_size, unsigned long number_elements);
bool dsp_ring_init(dsp_ring_t** __restrict c, char* block, size_t block_size, size_t element_size);
void dsp_ring_push(dsp_ring_t* __restrict c, const void* __restrict pData);
void dsp_ring_clear(dsp_ring_t* __restrict c);
uint32_t dsp_ring_capacity(dsp_ring_t* __restrict c);
void * dsp_ring_index(dsp_ring_t* __restrict c, uint32_t index);

#ifdef __cplusplus
} // extern C
#endif

#endif //guard
