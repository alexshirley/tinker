#include <string.h>
#include <cvector.h>

typedef struct cvector {
    uintptr_t capacity;
    size_t elem_size;
    void* last_entry;
    char block[1];
} cvector_t;

unsigned long cvector_required_size(unsigned long element_size, unsigned long number_elements) {
    return offsetof(cvector_t, block) + (element_size * number_elements);
}

bool cvector_init(cvector_t** vec, char* buffer, unsigned long block_size, unsigned long element_size) {
    if (block_size < offsetof(cvector_t, block)) {
        return false;
    }
    memset(buffer, 0, block_size);
    *vec               = (cvector_t*)buffer;
    (*vec)->capacity   = block_size - offsetof(cvector_t, block);
    (*vec)->last_entry = (*vec)->block;
    (*vec)->elem_size  = element_size;
    return true;
}

bool cvector_resize(cvector_t** vec, char* block, unsigned long block_size) {
    if (block_size < offsetof(cvector_t, block) + (*vec)->capacity) {
        return false;
    }
    memmove(block, (*vec), block_size);
    *vec = (cvector_t*)block;
    (*vec)->capacity = block_size - offsetof(cvector_t, block);
    return true;
}

bool cvector_push_back(cvector_t* vec, const void* value) {
    uintptr_t base_address = (uintptr_t)vec->last_entry;
    if (base_address >= vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    memcpy(vec->last_entry, value, vec->elem_size);
    uintptr_t next_entry = (uintptr_t)vec->last_entry + vec->elem_size;
    vec->last_entry      = (void*)next_entry;
    return true;
}

bool cvector_pop_back(cvector_t* vec, void* value) {
    uintptr_t last_entry = (uintptr_t)vec->last_entry;
    if (last_entry == (uintptr_t)vec->block) {
        return false;
    }
    memcpy(value, vec->last_entry, vec->elem_size);
    uintptr_t next_entry = (uintptr_t)vec->last_entry - vec->elem_size;
    vec->last_entry      = (void*)next_entry;
    return true;
}

bool cvector_get(cvector_t* vec, unsigned long element_index, void* value) {
    uintptr_t base_address  = (uintptr_t)vec->block;
    uintptr_t offset        = vec->elem_size * element_index;
    uintptr_t index_address = base_address + offset;
    if (index_address >= vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    if (index_address >= (uintptr_t)vec->last_entry) {
        return false;
    }
    memcpy(value, (void*)index_address, vec->elem_size);
    return true;
}

bool cvector_set(cvector_t* vec, unsigned long element_index, void* value) {
    uintptr_t base_address  = (uintptr_t)vec->block;
    uintptr_t offset        = vec->elem_size * element_index;
    uintptr_t index_address = base_address + offset;
    if (index_address >= vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    if (index_address >= (uintptr_t)vec->last_entry) {
        return false;
    }
    memcpy((void*)index_address, value, vec->elem_size);
    return true;
}

bool cvector_is_empty(cvector_t* vec) {
    return vec->last_entry == (void*)vec->block;
}

int cvector_capacity(cvector_t* vec) {
    return vec->capacity / vec->elem_size;
}

bool cvector_reserve(cvector_t* vec, size_t number_of_elements) {
    uintptr_t new_last_entry = (uintptr_t)vec->last_entry + number_of_elements * vec->elem_size;
    if (new_last_entry > vec->capacity + (uintptr_t)vec->block) {
        return false;
    }
    vec->last_entry = (void*)new_last_entry;
    return true;
}