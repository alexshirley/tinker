#include <greatest.h>
#include <non_blocking_ring.h>

#define NUM_ARR_ELE(x) (sizeof(x) / sizeof(x[0]))

typedef struct {
    int element_count;
    int element_size;
} init_closure;

TEST non_blocking_ring_creation_test(void* closure) {
    init_closure* args  = (init_closure*)closure;
    uint32_t alloc_size = non_blocking_ring_required_size(args->element_size, args->element_count);
    char* block         = malloc(alloc_size);
    non_blocking_ring_t* hdl;
    non_blocking_ring_init(&hdl, block, alloc_size, args->element_size);
    ASSERT_EQ(args->element_count, (int)non_blocking_ring_capacity(hdl));
    ASSERT_EQ(true, non_blocking_ring_empty(hdl));
    ASSERT_FALSE(non_blocking_ring_full(hdl));
    free(block);
    PASS();
}

typedef struct {
    non_blocking_ring_t* hdl;
} non_block_closure;

void non_blocking_test_setup(void* closure) {
    non_block_closure* args = (non_block_closure*)closure;
    uint32_t alloc_size     = non_blocking_ring_required_size(sizeof(uint16_t), 10);
    char* block             = malloc(alloc_size);
    non_blocking_ring_init(&args->hdl, block, alloc_size, sizeof(uint16_t));
}

void non_blocking_tear_down(void* closure) {
    non_block_closure* args = (non_block_closure*)closure;
    free(args->hdl);
}

TEST non_blocking_ring_push_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    uint16_t random_data     = 1024;
    ASSERT_EQ(true, non_blocking_ring_push(hdl, &random_data));
    ASSERT_EQ(false, non_blocking_ring_empty(hdl));
    ASSERT_EQ(false, non_blocking_ring_full(hdl));
    ASSERT_EQ(1, non_blocking_ring_count(hdl));
    PASS();
}

TEST non_blocking_ring_push_full_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    for (int i = 0; i < 10; i++) {
        const uint16_t random_data = i * 7 % 29;
        ASSERT_EQ(true, non_blocking_ring_push(hdl, &random_data));
    }
    ASSERT_EQ(true, non_blocking_ring_full(hdl));
    PASS();
}

TEST non_blocking_ring_pop_ret_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    uint16_t data            = 257;
    (void)non_blocking_ring_push(hdl, &data);
    uint16_t ret;
    ASSERT_EQ(true, non_blocking_ring_pop(hdl, &ret));
    ASSERT_EQ(data, ret);
    PASS();
}

TEST non_blocking_ring_pop_ret_fail_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    uint16_t ret;
    ASSERT_EQ(false, non_blocking_ring_pop(hdl, &ret));
    PASS();
}

TEST non_blocking_index_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    ASSERT_EQ(NULL, non_blocking_ring_index(hdl, 1));
    for (uint16_t i = 0; i < 10; i++) {
        non_blocking_ring_push(hdl, &i);
    }
    ASSERT_EQ(NULL, non_blocking_ring_index(hdl, 10));

    for (uint16_t i = 0; i < 10; i++) {
        uint16_t* retP = (uint16_t*)non_blocking_ring_index(hdl, i);
        ASSERT(retP != NULL);
        ASSERT_EQ(i, *retP);
    }
    PASS();
}

TEST non_blocking_ring_clear_test(void* closure) {
    non_block_closure* args  = (non_block_closure*)closure;
    non_blocking_ring_t* hdl = args->hdl;
    for (int i = 0; i < 10; i++) {
        const uint16_t random_data = i * 7 % 29;
        non_blocking_ring_push(hdl, &random_data);
    }

    non_blocking_ring_clear(hdl);
    ASSERT_EQ(true, non_blocking_ring_empty(hdl));
    ASSERT_EQ(false, non_blocking_ring_full(hdl));
    int throwaway;
    ASSERT_EQ(false, non_blocking_ring_pop(hdl, &throwaway));

    PASS();
}

SUITE(non_blocking_ring_suite) {
    int element_sizes[]  = {1, 2, 4, 8, 17};    // Common Sizes of elements + one odd size to simulate struct
    int element_counts[] = {1, 2, 4, 16, 1024}; // Common Buffer Element Capacities
    for (int i = 0; i < NUM_ARR_ELE(element_sizes); i++) {
        for (int j = 0; j < NUM_ARR_ELE(element_counts); j++) {
            init_closure obj = {.element_count = element_counts[j], .element_size = element_sizes[i]};
            RUN_TESTp(non_blocking_ring_creation_test, &obj);
        }
    }
    non_block_closure args;
    SET_SETUP(non_blocking_test_setup, &args);
    SET_TEARDOWN(non_blocking_tear_down, &args);
    RUN_TESTp(non_blocking_ring_push_test, &args);
    RUN_TESTp(non_blocking_ring_push_full_test, &args);
    RUN_TESTp(non_blocking_ring_pop_ret_test, &args);
    RUN_TESTp(non_blocking_ring_pop_ret_fail_test, &args);
    RUN_TESTp(non_blocking_index_test, &args);
    RUN_TESTp(non_blocking_ring_clear_test, &args);
}