#include <dsp_ring.h>
#include <greatest.h>

#define NUM_ARR_ELE(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
    size_t element_count;
    size_t element_size;
} init_closure;

TEST dsp_ring_creation_test(void* closure) {
    init_closure* args  = (init_closure*)closure;
    uint32_t alloc_size = dsp_ring_required_size(args->element_size, args->element_count);
    char* block         = malloc(alloc_size);
    dsp_ring_t* hdl;
    dsp_ring_init(&hdl, block, alloc_size, args->element_size);
    ASSERT_EQ(args->element_count, dsp_ring_capacity(hdl));
    free(block);
    PASS();
}

typedef struct {
    dsp_ring_t* hdl;
} dsp_closure;

void dsp_test_setup(void* closure) {
    dsp_closure* args   = (dsp_closure*)closure;
    uint32_t alloc_size = dsp_ring_required_size(sizeof(uint16_t), 10);
    char* block         = malloc(alloc_size);
    dsp_ring_init(&args->hdl, block, alloc_size, sizeof(uint16_t));
}

void dsp_teardown(void* closure) {
    dsp_closure* args = (dsp_closure*)closure;
    free(args->hdl);
}

TEST dsp_empty_test(void* closure) {
    dsp_closure* args = (dsp_closure*)closure;
    dsp_ring_t* hdl   = args->hdl;
    for (unsigned i = 0; i < dsp_ring_capacity(hdl); i++) {
        uint16_t* grab = dsp_ring_index(hdl, i);
        ASSERT(NULL != grab);
        ASSERT_EQ(0, *grab);
    }
    PASS();
}

TEST dsp_push_test(void* closure) {
    dsp_closure* args = (dsp_closure*)closure;
    dsp_ring_t* hdl   = args->hdl;
    for (uint16_t i = 0; i < (uint16_t)dsp_ring_capacity(hdl); i++) {
        dsp_ring_push(hdl, &i);
    }
    for (unsigned i = 0; i < dsp_ring_capacity(hdl); i++) {
        uint16_t* grab = dsp_ring_index(hdl, i);
        ASSERT(NULL != grab);
        ASSERT_EQ(i, *grab);
    }
    PASS();
}

TEST dsp_clear_test(void* closure) {
    dsp_closure* args = (dsp_closure*)closure;
    dsp_ring_t* hdl   = args->hdl;
    for (uint16_t i = 0; i < (uint16_t)dsp_ring_capacity(hdl); i++) {
        dsp_ring_push(hdl, &i);
    }
    dsp_ring_clear(hdl);
    for (unsigned i = 0; i < dsp_ring_capacity(hdl); i++) {
        uint16_t* grab = dsp_ring_index(hdl, i);
        ASSERT(NULL != grab);
        ASSERT_EQ(0, *grab);
    }
    PASS();
}

SUITE(dsp_ring_suite) {
    int element_sizes[]  = {1, 2, 4, 8, 17};    // Common Sizes of elements + one odd size to simulate struct
    int element_counts[] = {1, 2, 4, 16, 1024}; // Common Buffer Element Capacities
    for (unsigned i = 0; i < NUM_ARR_ELE(element_sizes); i++) {
        for (unsigned j = 0; j < NUM_ARR_ELE(element_counts); j++) {
            init_closure obj = {.element_count = (size_t)element_counts[j], .element_size = (size_t)element_sizes[i]};
            RUN_TESTp(dsp_ring_creation_test, &obj);
        }
    }
    dsp_closure args; // Using uint16_t cap 10 from here on out
    SET_SETUP(dsp_test_setup, &args);
    SET_TEARDOWN(dsp_teardown, &args);
    RUN_TESTp(dsp_empty_test, &args);
    RUN_TESTp(dsp_push_test, &args);
    RUN_TESTp(dsp_clear_test, &args);
}
