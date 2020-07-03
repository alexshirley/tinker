#include <greatest.h>
#include <cvector.h>
#include <stdint.h>

TEST dummy_test() {
    PASS();
}

TEST cvector_push_test() {
    // We'll store ints
    uint32_t alloc_size = cvector_required_size(sizeof(int), 10);
    char* cvec_buff     = malloc(alloc_size);
    cvector_t* cvec_hdl;
    ASSERT_EQ(true, cvector_init(&cvec_hdl, cvec_buff, alloc_size, sizeof(int)));
    ASSERT_EQ(true, cvector_is_empty(cvec_hdl));
    ASSERT_EQ(10, cvector_capacity(cvec_hdl));
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(true, cvector_push_back(cvec_hdl, &i));
    }
    for (int i = 0; i < 10; i++) {
        int temp;
        ASSERT_EQ(true, cvector_get(cvec_hdl, i, &temp));
        ASSERT_EQ(i, temp);
    }
    free(cvec_buff);
    PASS();
}

TEST cvector_pop_test() {
    // We'll store ints
    uint32_t alloc_size = cvector_required_size(sizeof(int), 10);
    char* cvec_buff     = malloc(alloc_size);
    cvector_t* cvec_hdl;
    ASSERT_EQ(true, cvector_init(&cvec_hdl, cvec_buff, alloc_size, sizeof(int)));
    ASSERT_EQ(true, cvector_is_empty(cvec_hdl));
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(true, cvector_push_back(cvec_hdl, &i));
    }
    ASSERT_FALSE(cvector_is_empty(cvec_hdl));
    for (int i = 9; i != -1; i--) {
        int temp;
        ASSERT_EQ(true, cvector_pop_back(cvec_hdl, &temp));
        ASSERT_EQ_FMT(i, temp, "%d");
    }
    ASSERT_EQ(true, cvector_is_empty(cvec_hdl));
    free(cvec_buff);
    PASS();
}

TEST cvector_set_test() {
    uint32_t alloc_size = cvector_required_size(sizeof(int), 10);
    char* cvec_buff     = malloc(alloc_size);
    cvector_t* cvec_hdl;
    ASSERT_EQ(true, cvector_init(&cvec_hdl, cvec_buff, alloc_size, sizeof(int)));
    ASSERT_EQ(true, cvector_reserve(cvec_hdl, 1));
    int testval = 1;
    ASSERT_EQ(true, cvector_set(cvec_hdl, 0, &testval));
    ASSERT_EQ(true, cvector_reserve(cvec_hdl, 9));
    for (int i = 0; i < 10; i++) {
        cvector_set(cvec_hdl, i, &i);
    }
    ASSERT_FALSE(cvector_reserve(cvec_hdl, 1));
    free(cvec_buff);
    PASS();
}

TEST cvector_resize_test() {
    char block[1000];
    int alloc_size = cvector_required_size(sizeof(uint16_t), 10);
    cvector_t* hdl;
    ASSERT_EQ(true, cvector_init(&hdl, block, alloc_size, sizeof(uint16_t)));
    ASSERT_EQ(true, cvector_reserve(hdl, 1));
    uint16_t testval = 1;
    ASSERT_EQ(true, cvector_set(hdl, 0, &testval));

    // Try resizing to use the full block
    ASSERT_EQ(true, cvector_resize(&hdl, block, 1000));
    int capacity = cvector_capacity(hdl);
    ASSERT(capacity > 400);
    // Peek the value we set previously
    uint16_t peek;
    ASSERT_EQ(true, cvector_get(hdl, 0, &peek));
    ASSERT_EQ(1, peek);
    peek = 2;
    ASSERT_EQ(true, cvector_push_back(hdl, &peek));

    PASS();
}

TEST cvector_resize_realloc() {
    char* block     = malloc(1000);
    int needed_size = cvector_required_size(sizeof(uint32_t), 10);
    cvector_t* hdl;
    ASSERT_EQ(true, cvector_init(&hdl, block, needed_size, sizeof(uint32_t)));
    uint32_t testval = 0xdeadbeef;
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(true, cvector_push_back(hdl, &testval));
    }
    block = realloc(block, 2000);
    ASSERT_EQ(true, cvector_resize(&hdl, block, 2000));
    // // Peek the value we set previously
    uint32_t peek;
    ASSERT_EQ(true, cvector_get(hdl, 0, &peek));
    ASSERT_EQ(0xdeadbeef, peek);
    peek = 2;
    ASSERT_EQ(true, cvector_push_back(hdl, &peek));
    free(block);
    PASS();
}

TEST cvector_size_test() {
    char* block     = malloc(1000);
    int needed_size = cvector_required_size(sizeof(uint32_t), 10);
    cvector_t* hdl;
    ASSERT_EQ(true, cvector_init(&hdl, block, needed_size, sizeof(uint32_t)));
    ASSERT_EQ(0, cvector_size(hdl));
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(true, cvector_push_back(hdl, &i));
    }
    ASSERT_EQ(5, cvector_size(hdl));
    PASS();
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

TEST cvector_swap_ref_test() {
    char block[1000];
    cvector_t* hdl;
    ASSERT_EQ(true, cvector_init(&hdl, block, 1000, sizeof(int)));

    for (int i = 0; i < 10; i++) {
        cvector_push_back(hdl, &i);
    }

    for (int i = 0; i < 5; i++) {
        swap(cvector_get_ref(hdl, i), cvector_get_ref(hdl, 10 - i - 1), sizeof(int));
    }

    for (int i = 0; i < 10; i++) {
        int trash;
        cvector_get(hdl, i, &trash);
        ASSERT_EQ_FMTm("Should be equal", 10 - i - 1, trash, "%d");
    }

    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(cvector_push_test);
    RUN_TEST(cvector_pop_test);
    RUN_TEST(cvector_set_test);
    RUN_TEST(cvector_resize_test);
    RUN_TEST(cvector_resize_realloc);
    RUN_TEST(cvector_size_test);
    RUN_TEST(cvector_swap_ref_test);

    GREATEST_MAIN_END(); /* display results */
}