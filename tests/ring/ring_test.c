#include <ring.h>
#include <greatest.h>

TEST dummy_test() {
    PASS();
}

TEST ring_creation_test(){
    uint32_t alloc_size = ring_required_size(sizeof(uint16_t),8);
    char * block = malloc(alloc_size);
    ring_t * hdl;
    ring_init(&hdl, block, alloc_size, sizeof(uint16_t));
    ASSERT_EQ(true, ring_empty(hdl));
    ASSERT_FALSE(ring_full(hdl));
    for (uint16_t i = 0; i < 10; i++){
        ring_push(hdl,&i);
    }
    ASSERT_EQ(true,ring_full(hdl));
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