#include <blocking_ring.h>
#include <stddef.h>
#include <string.h>

typedef struct blocking_ring {
    size_t element_size;
    size_t capacity;
    char* push_p;
    char* pop_p;
    bool full;
    char data[1];
} blocking_ring_t;

size_t blocking_ring_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(blocking_ring_t, data) + (element_size * number_elements);
}

bool blocking_ring_init(blocking_ring_t** __restrict c, char* block, size_t block_size, size_t element_size) {
    if (block_size < offsetof(blocking_ring_t, data)) {
        return false;
    }
    if (element_size == 0) {
        return false;
    }
    memset(block, 0, block_size);
    *c                 = (blocking_ring_t*)block;
    (*c)->pop_p        = (*c)->data;
    (*c)->push_p       = (*c)->data;
    (*c)->full         = false;
    (*c)->element_size = element_size;
    (*c)->capacity     = (block_size - offsetof(blocking_ring_t, data)) / element_size;
    return true;
}

size_t blocking_ring_capacity(blocking_ring_t* __restrict c) {
    return c->capacity;
}

size_t blocking_ring_count(blocking_ring_t* __restrict c) {
    if (c->full) {
        return c->capacity;
    }
    const ptrdiff_t pop_p       = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p      = (ptrdiff_t)c->push_p;
    const ptrdiff_t data_offset = (push_p - pop_p);
    const size_t actual_data    = (data_offset > 0) ? (size_t)data_offset : c->capacity * c->element_size - (size_t)data_offset;
    return actual_data / c->element_size;
}

bool blocking_ring_empty(blocking_ring_t* __restrict c) {
    const ptrdiff_t pop_p  = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p = (ptrdiff_t)c->push_p;
    return (push_p == pop_p && !c->full);
}

bool blocking_ring_full(blocking_ring_t* __restrict c) {
    return c->full;
}

void blocking_ring_clear(blocking_ring_t* __restrict c) {
    c->full  = false;
    c->pop_p = c->push_p;
}

static char* inc_head(blocking_ring_t* __restrict c, char* __restrict ptr) {
    const size_t cStructSize  = c->element_size;
    const size_t cNumElements = c->capacity;
    const uintptr_t payload   = (uintptr_t)c->data;
    const uintptr_t max       = (uintptr_t)(payload + cNumElements * cStructSize);
    return ((uintptr_t)(ptr + cStructSize) >= max) ? (char*)payload : (char*)(ptr + cStructSize);
}

bool blocking_ring_push(blocking_ring_t* __restrict c, const void* __restrict pData) {
    if (c->full) {
        return false;
    }
    memcpy(c->push_p, pData, c->element_size);
    c->push_p = (char*)inc_head(c, c->push_p);
    c->full   = (c->pop_p == c->push_p);
    return true;
}

bool blocking_ring_pop(blocking_ring_t* __restrict c, void* __restrict pData) {
    if (blocking_ring_empty(c)) {
        return false;
    }
    c->full = false;
    memcpy(pData, c->pop_p, c->element_size);
    c->pop_p = (char*)inc_head(c, c->pop_p);
    return true;
}

void* blocking_ring_index(blocking_ring_t* __restrict c, uint32_t index) {
    if (blocking_ring_empty(c)) {
        return NULL;
    }
    if (blocking_ring_count(c) <= index) {
        return NULL;
    }
    const size_t max_address = (size_t)c->data + c->capacity * c->element_size;
    size_t v_index_address   = (size_t)c->pop_p + index * c->element_size;
    size_t offset            = (v_index_address >= max_address) ? v_index_address - max_address + (size_t)c->data : v_index_address;
    return (void*)offset;
}
