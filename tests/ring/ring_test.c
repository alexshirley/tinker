#include <non_blocking_ring.h>
#include <greatest.h>

TEST dummy_test() {
    PASS();
}

TEST non_blocking_ring_creation_test(){
    uint32_t alloc_size = non_blocking_ring_required_size(sizeof(uint16_t),8);
    char * block = malloc(alloc_size);
    ring_t * hdl;
    non_blocking_ring_init(&hdl, block, alloc_size, sizeof(uint16_t));
    ASSERT_EQ(true, non_blocking_ring_empty(hdl));
    ASSERT_FALSE(non_blocking_ring_full(hdl));
    free(block);
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(dummy_test);
    RUN_TEST(ring_creation_test);

    GREATEST_MAIN_END(); /* display results */
}