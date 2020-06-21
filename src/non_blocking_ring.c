#include <non_blocking_ring.h>
#include <stddef.h>
#include <string.h>

typedef struct ring {
    size_t element_size;
    size_t capacity;
    char* __restrict push_p;
    char* __restrict pop_p;
    bool full;
    char data[1];
} non_blocking_ring_t;

unsigned long non_blocking_ring_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(non_blocking_ring_t, data) + (element_size * number_elements);
}

bool non_blocking_ring_init(non_blocking_ring_t** __restrict c, char* block, size_t block_size, size_t element_size) {
    if (block_size < offsetof(non_blocking_ring_t, data)) {
        return false;
    }
    if (element_size < 1) {
        return false;
    }
    memset(block, 0, block_size);
    *c                 = (non_blocking_ring_t*)block;
    (*c)->pop_p        = (*c)->data;
    (*c)->push_p       = (*c)->data;
    (*c)->full         = false;
    (*c)->element_size = element_size;
    (*c)->capacity     = (block_size - offsetof(non_blocking_ring_t, data)) / element_size;
    return true;
}

uint32_t non_blocking_ring_capacity(non_blocking_ring_t* __restrict c) {
    return c->capacity;
}

uint32_t non_blocking_ring_count(non_blocking_ring_t* __restrict c) {
    if (c->full) {
        return c->capacity;
    }
    const ptrdiff_t pop_p       = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p      = (ptrdiff_t)c->push_p;
    const ptrdiff_t data_offset = (push_p - pop_p);
    const ptrdiff_t actual_data = (data_offset > 0) ? data_offset : c->capacity * c->element_size - data_offset;
    return actual_data / c->element_size;
}

bool non_blocking_ring_empty(non_blocking_ring_t* __restrict c) {
    const ptrdiff_t pop_p  = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p = (ptrdiff_t)c->push_p;
    return (push_p == pop_p && !c->full);
}

bool non_blocking_ring_full(non_blocking_ring_t* __restrict c) {
    return c->full;
}

void non_blocking_ring_clear(non_blocking_ring_t* __restrict c) {
    c->full = false;
    c->pop_p  = c->push_p;
}

static char* inc_head(non_blocking_ring_t* __restrict c, char* __restrict ptr) {
    const size_t cStructSize  = c->element_size;
    const size_t cNumElements = c->capacity;
    const uintptr_t payload   = (uintptr_t)c->data;
    const uintptr_t max       = (uintptr_t)(payload + cNumElements * cStructSize);
    return ((uintptr_t)(ptr + cStructSize) >= max) ? (char*)payload : (char*)(ptr + cStructSize);
}

bool non_blocking_ring_push(non_blocking_ring_t* __restrict c, const void* __restrict pData) {
    memcpy(c->push_p, pData, c->element_size);
    if (c->full) {
        c->pop_p = (char*)inc_head(c, c->pop_p);
    }
    c->push_p = (char*)inc_head(c, c->push_p);
    c->full   = (c->pop_p == c->push_p);
    return true; // We return true, because this can never fail, however comm buffering should fail
}

bool non_blocking_ring_pop(non_blocking_ring_t* __restrict c, void* __restrict pData) {
    if (non_blocking_ring_empty(c)) {
        return false;
    }
    c->full = false;
    memcpy(pData, c->pop_p, c->element_size);
    c->pop_p = (char*)inc_head(c, c->pop_p);
    return true;
}

void* non_blocking_ring_index(non_blocking_ring_t* __restrict c, uint32_t index) {
    if (non_blocking_ring_empty(c)) {
        return NULL;
    }
    if (non_blocking_ring_count(c) <= index) {
        return NULL;
    }
    const ptrdiff_t max_address = (ptrdiff_t)c->data + c->capacity * c->element_size;
    ptrdiff_t v_index_address   = (ptrdiff_t)c->pop_p + index * c->element_size;
    ptrdiff_t offset            = (v_index_address >= max_address) ? v_index_address - max_address + (ptrdiff_t)c->data : v_index_address;
    return (void*)offset;
}
