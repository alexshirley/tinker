#include <priority_queue.h>
#include <cvector.h>
#include <string.h>

static void swap(void* __restrict a, void* __restrict b, size_t len);

typedef struct prio_q {
    compare func;
    cvector_t* heap;
} prio_q_t;

bool prio_q_peek(prio_q_t* __restrict p, void* out) {
    return cvector_get(p->heap, 0, &out);
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
    if (!cvector_init(&vec, block + sizeof(prio_q_t), block_size - sizeof(prio_q_t), element_size)) {
        return false;
    }
    (*p)->heap = vec;
    return true;
}

static void siftUp(prio_q_t* q, int currentIdx) {
    int parentIdx           = (currentIdx - 1) / 2;
    cvector_t* __restrict h = q->heap;
    void* current_addy      = cvector_get_ref(h, currentIdx);
    void* parent_addy       = cvector_get_ref(h, parentIdx);
    while (currentIdx > 0 && q->func(current_addy, parent_addy)) {
        swap(current_addy, parent_addy, cvector_element_size(h));
        currentIdx   = parentIdx;
        parentIdx    = (currentIdx - 1) / 2;
        current_addy = cvector_get_ref(h, currentIdx);
        parent_addy  = cvector_get_ref(h, parentIdx);
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
