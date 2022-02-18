#include <string.h>
#include <cvector.h>

typedef struct cvector {
    size_t capacity;  // in bytes
    size_t elem_size; // in bytes
    size_t used;
    char* last_entry;
    char block[1];
} cvector_t;

unsigned long cvector_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(cvector_t, block) + (element_size * number_elements);
}

bool cvector_init(cvector_t** __restrict vec, char* __restrict buffer, unsigned long block_size, unsigned long element_size) {
    if (block_size < offsetof(cvector_t, block)) {
        return false;
    }
    memset(buffer, 0, block_size);
    *vec               = (cvector_t*)buffer;
    (*vec)->capacity   = block_size - offsetof(cvector_t, block);
    (*vec)->last_entry = (*vec)->block;
    (*vec)->elem_size  = element_size;
    (*vec)->used = 0;
    return true;
}

bool cvector_resize(cvector_t** __restrict vec, char* __restrict block, unsigned long block_size) {
    size_t oldSize = offsetof(cvector_t, block) + (*vec)->capacity;
    if (block_size < oldSize) {
        return false;
    }
    ptrdiff_t distance = (*vec)->last_entry - (*vec)->block;
    *vec               = memmove(block, (*vec), oldSize);
    (*vec)->capacity   = block_size - offsetof(cvector_t, block);
    (*vec)->last_entry = (*vec)->block + distance;
    return true;
}

bool cvector_push_back(cvector_t* __restrict vec, const void* __restrict value) {
    uintptr_t base_address = (uintptr_t)vec->last_entry;
    if (base_address >= vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    memcpy(vec->last_entry, value, vec->elem_size);
    vec->last_entry += vec->elem_size;
    vec->used += 1;
    return true;
}

bool cvector_pop_back(cvector_t* __restrict vec, void* __restrict value) {
    if (vec->last_entry == vec->block)
        return false;
    vec->last_entry -= vec->elem_size;
    vec->used -= 1;
    memcpy(value, vec->last_entry, vec->elem_size);
    return true;
}

bool cvector_get(const cvector_t* vec, unsigned long element_index, void* value) {
    const void* address = cvector_get_ref(vec, element_index);
    if (address == NULL) {
        return false;
    }
    memcpy(value, (void*)address, vec->elem_size);
    return true;
}

bool cvector_set(cvector_t* __restrict vec, unsigned long element_index, const void* value) {
    void* address = cvector_get_ref(vec, element_index);
    if (address == NULL) {
        return false;
    }
    memcpy((void*)address, value, vec->elem_size);
    return true;
}

bool cvector_is_empty(const cvector_t* vec) {
    return vec->last_entry == (void*)vec->block;
}

int cvector_capacity(const cvector_t* vec) {
    return vec->capacity / vec->elem_size;
}

bool cvector_reserve(cvector_t* __restrict vec, size_t number_of_elements) {
    uintptr_t new_last_entry = (uintptr_t)vec->last_entry + number_of_elements * vec->elem_size;
    if (new_last_entry > vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    vec->last_entry = (void*)new_last_entry;
    return true;
}

void* cvector_get_ref(const cvector_t* vec, unsigned long element_index) {
    uintptr_t base_address  = (uintptr_t)vec->block;
    uintptr_t offset        = vec->elem_size * element_index;
    uintptr_t index_address = base_address + offset;
    if (index_address >= vec->capacity + (uintptr_t)vec->block) {
        return NULL;
    }
    if (index_address >= (uintptr_t)vec->last_entry) {
        return NULL;
    }
    return (void*)index_address;
}

size_t cvector_element_size(const cvector_t* v) {
    return v->elem_size;
}

size_t cvector_size(const cvector_t* v) {
    return v->used;
}
