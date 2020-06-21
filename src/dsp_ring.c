#include <dsp_ring.h>
#include <stddef.h>
#include <string.h>

typedef struct dsp_ring dsp_ring_t;
typedef struct dsp_ring {
    size_t element_size;
    size_t capacity;
    char* __restrict push_p;
    char data[1];
} dsp_ring_t;

unsigned long dsp_ring_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(dsp_ring_t, data) + (element_size * number_elements);
}
bool dsp_ring_init(dsp_ring_t** __restrict c, char* block, size_t block_size, size_t element_size) {
    if (block_size < offsetof(dsp_ring_t, data)) {
        return false;
    }
    if (element_size < 1) {
        return false;
    }
    memset(block, 0, block_size);
    *c                 = (dsp_ring_t*)block;
    (*c)->push_p       = (*c)->data;
    (*c)->element_size = element_size;
    (*c)->capacity     = (block_size - offsetof(dsp_ring_t, data)) / element_size;
    return true;
}

static char* inc_head(dsp_ring_t* __restrict c, char* __restrict ptr) {
    const size_t cStructSize  = c->element_size;
    const size_t cNumElements = c->capacity;
    const uintptr_t payload   = (uintptr_t)c->data;
    const uintptr_t max       = (uintptr_t)(payload + cNumElements * cStructSize);
    return ((uintptr_t)(ptr + cStructSize) >= max) ? (char*)payload : (char*)(ptr + cStructSize);
}

void dsp_ring_push(dsp_ring_t* __restrict c, const void* __restrict pData) {
    memcpy(c->push_p, pData, c->element_size);
    c->push_p = (char*)inc_head(c, c->push_p);
}

void dsp_ring_clear(dsp_ring_t* __restrict c) {
    memset(c->data, 0, c->element_size * c->capacity);
}

uint32_t dsp_ring_capacity(dsp_ring_t* __restrict c) {
    return c->capacity;
}

void* dsp_ring_index(dsp_ring_t* __restrict c, uint32_t index) {
    const ptrdiff_t max_address = (ptrdiff_t)c->data + c->capacity * c->element_size;
    ptrdiff_t v_index_address   = (ptrdiff_t)c->push_p + index * (c->element_size);
    ptrdiff_t offset            = (v_index_address >= max_address) ? v_index_address - max_address + (ptrdiff_t)c->data : v_index_address;
    return (void*)offset;
}