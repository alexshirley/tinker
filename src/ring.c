#include <ring.h>
#include <stddef.h>
#include <string.h>

typedef struct ring {
    size_t element_size;
    size_t capacity;
    char* __restrict push_p;
    char* __restrict pop_p;
    bool full;
    char data[1];
} ring_t;

unsigned long ring_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(ring_t, data) + (element_size * number_elements);
}

bool ring_init(ring_t** __restrict c, char* block, size_t block_size, size_t element_size) {
    if (block_size < offsetof(ring_t, data)) {
        return false;
    }
    if (element_size < 1) {
        return false;
    }
    memset(block, 0, block_size);
    *c                 = (ring_t*)block;
    (*c)->pop_p        = (*c)->data;
    (*c)->push_p       = (*c)->data;
    (*c)->full         = false;
    (*c)->element_size = element_size;
    (*c)->capacity     = (block_size - offsetof(ring_t, data)) / element_size;
    return true;
}

uint32_t ring_capacity(ring_t* __restrict c) {
    return c->capacity;
}

uint32_t ring_count(ring_t* __restrict c) {
    if (c->full) {
        return c->capacity;
    }
    const ptrdiff_t pop_p  = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p = (ptrdiff_t)c->push_p;
    const ptrdiff_t offset = (push_p - pop_p) / c->element_size;
    return (offset > 0) ? offset : c->capacity - offset;
}

bool ring_empty(ring_t* __restrict c) {
    const ptrdiff_t pop_p  = (ptrdiff_t)c->pop_p;
    const ptrdiff_t push_p = (ptrdiff_t)c->push_p;
    return (push_p == pop_p && !c->full);
}

bool ring_full(ring_t* __restrict c) {
    return c->full;
}

void ring_clear(ring_t* __restrict c) {
    c->full   = false;
    c->pop_p  = c->data;
    c->push_p = c->data;
}

static uintptr_t pointer_inc(uintptr_t ptr, uintptr_t base, uintptr_t limit, size_t elem_size) {
    return (ptr += elem_size >= limit) ? base : ptr;
}

bool ring_push(ring_t* __restrict c, const void* __restrict pData) {
    memcpy(c->push_p, pData, c->element_size);
    uintptr_t max_address = (uintptr_t)c->data + c->element_size * c->capacity;
    if (c->full)
        c->pop_p = (char*)pointer_inc((ptrdiff_t)c->pop_p, (ptrdiff_t)c->data, max_address, c->element_size);
    c->push_p = (char*)pointer_inc((ptrdiff_t)c->push_p, (ptrdiff_t)c->data, max_address, c->element_size);
    c->full   = (c->pop_p == c->push_p);
    return true; // We return true, because this can never fail, however comm buffering should fail
}

bool ring_pop(ring_t* __restrict c, void* __restrict pData) {
    if (ring_empty(c)) {
        return false;
    }
    c->full = false;
    memcpy(pData, c->pop_p, c->element_size);
    uintptr_t max_address = (uintptr_t)c->data + c->element_size * c->capacity;
    c->pop_p              = (char*)pointer_inc((uintptr_t)c->pop_p, (uintptr_t)c->data, max_address, c->element_size);
    return true;
}
