#include <priority_queue.h>
#include <cvector.h>
#include <string.h>

static void swap(void* __restrict a, void* __restrict b, size_t len);
static int childL_idx(int parent) {
    return parent * 2 + 1;
}
static int childR_idx(int parent) {
    return parent * 2 + 2;
}
static int parent_idx(int child) {
    return (child - 1) / 2;
}

typedef struct prio_q {
    compare func;
    cvector_t* heap;
} prio_q_t;

bool prio_q_peek(prio_q_t* __restrict p, void* out) {
    return cvector_get(p->heap, 0, out);
}

size_t prio_q_required_size(unsigned long element_size, unsigned long number_elements) {
    return sizeof(prio_q_t) + cvector_required_size(element_size, number_elements);
}

bool prio_q_init(prio_q_t** p, char* block, unsigned long block_size, unsigned long element_size, compare func) {
    if (block_size < prio_q_required_size(0, 0)) {
        return false;
    }
    (*p)       = (prio_q_t*)&block[0];
    (*p)->func = func;
    cvector_t* vec;
    const size_t remaining = block_size - sizeof(prio_q_t);
    if (!cvector_init(&vec, &block[sizeof(prio_q_t)], remaining, element_size)) {
        (*p) = NULL;
        return false;
    }
    (*p)->heap = vec;
    return true;
}

static void siftUp(prio_q_t* q, int currentIdx) {
    int parentIdx           = (currentIdx - 1) / 2;
    cvector_t* __restrict h = q->heap;
    void* current_ref       = cvector_get_ref(h, currentIdx);
    void* parent_ref        = cvector_get_ref(h, parentIdx);
    while (currentIdx > 0 && q->func(current_ref, parent_ref) < 0) {
        swap(current_ref, parent_ref, cvector_element_size(h));
        currentIdx  = parentIdx;
        parentIdx   = (currentIdx - 1) / 2;
        current_ref = cvector_get_ref(h, currentIdx);
        parent_ref  = cvector_get_ref(h, parentIdx);
    }
}

bool prio_q_insert(prio_q_t* q, void* value) {
    if (false == cvector_push_back(q->heap, value)) {
        return false;
    }
    siftUp(q, cvector_size(q->heap) - 1);
    return true;
}

static void swap(void* __restrict a, void* __restrict b, size_t len) {
    char* __restrict p = (char*)a;
    char* __restrict q = (char*)b;
    while (len--) {
        char tmp = *p;
        *p++     = *q;
        *q++     = tmp;
    };
}

static void siftDown(prio_q_t* q, int currentIdx, int endIdx) {
    cvector_t* __restrict h = q->heap;
    for (int childL_pos = childL_idx(currentIdx); childL_pos <= endIdx;) {
        const int childR_pos         = childR_idx(currentIdx) <= endIdx ? childR_idx(currentIdx) : -1;
        void* __restrict childR_ref  = cvector_get_ref(h, childR_pos);
        void* __restrict childL_ref  = cvector_get_ref(h, childL_pos);
        const int swap_idx           = (childR_pos != -1 && q->func(childR_ref, childL_ref) < 0) ? childR_pos : childL_pos;
        void* __restrict swap_ref    = cvector_get_ref(h, swap_idx);
        void* __restrict current_ref = cvector_get_ref(h, currentIdx);
        if (q->func(swap_ref, current_ref) >= 0) {
            return;
        }
        swap(current_ref, swap_ref, cvector_element_size(h));
        currentIdx = swap_idx;
        childL_pos = childL_idx(currentIdx);
    }
}

bool prio_q_remove(prio_q_t* q, void* value) {
    cvector_t* __restrict h = q->heap;
    const size_t h_size     = cvector_size(h);
    if (!h_size) { // if we have no elements, early terminate
        return false;
    }
    swap(cvector_get_ref(h, 0), cvector_get_ref(h, h_size - 1), cvector_element_size(h));
    cvector_pop_back(h, value);
    siftDown(q, 0, cvector_size(h) - 1);
    return true;
}