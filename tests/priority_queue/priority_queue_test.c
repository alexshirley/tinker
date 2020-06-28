#include <priority_queue.h>
#include <greatest.h>

int pq_comp_min(const void* a, const void* b) {
    int val1 = *(int*)a;
    int val2 = *(int*)b;
    return (val1 > val2) - (val1 < val2);
}

TEST pq_creation_test() {
    char buf[1000];
    prio_q_t* hdl;
    ASSERT_EQ(true, prio_q_init(&hdl, buf, 1000, sizeof(int), pq_comp_min));
    int trash;
    ASSERT_EQ(false, prio_q_peek(hdl, &trash));
    PASS();
}

TEST pq_insert_test() {
    char buf[1000];
    prio_q_t* hdl;
    ASSERT_EQ(true, prio_q_init(&hdl, buf, 1000, sizeof(int), pq_comp_min));
    int trash;
    ASSERT_EQ(false, prio_q_peek(hdl, &trash));

    int test[] = {34, 23, 45};
    ASSERT_EQ(true, prio_q_insert(hdl, &test[0])); // Insert 34
    ASSERT_EQ(true, prio_q_peek(hdl, &trash));
    ASSERT_EQ_FMTm("Should be equal", test[0], trash, "%d"); // Expect 34

    ASSERT_EQ(true, prio_q_insert(hdl, &test[1])); // Add 23
    ASSERT_EQ(true, prio_q_peek(hdl, &trash));
    ASSERT_EQ_FMTm("Should be equal", test[1], trash, "%d"); // Expect 23

    ASSERT_EQ(true, prio_q_insert(hdl, &test[2])); // Add 45
    ASSERT_EQ(true, prio_q_peek(hdl, &trash));
    ASSERT_EQ_FMTm("Should be equal", test[1], trash, "%d"); // Expect 23

    PASS();
}

TEST pq_pop_test() {
    char buf[1000];
    prio_q_t* hdl;
    ASSERT_EQ(true, prio_q_init(&hdl, buf, 1000, sizeof(int), pq_comp_min));

    int test[] = {17, 199, 8, 8};
    for (int i = 0; i < 4; i++) {
        (void)prio_q_insert(hdl, &test[i]);
    }
    int result;
    ASSERT_EQ(true, prio_q_remove(hdl, &result));
    ASSERT_EQ_FMT(8, result, "%d");
    ASSERT_EQ(true, prio_q_remove(hdl, &result));
    ASSERT_EQ_FMT(8, result, "%d");
    ASSERT_EQ(true, prio_q_remove(hdl, &result));
    ASSERT_EQ_FMT(17, result, "%d");
    ASSERT_EQ(true, prio_q_remove(hdl, &result));
    ASSERT_EQ_FMT(199, result, "%d");
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(pq_creation_test);
    RUN_TEST(pq_insert_test);
    RUN_TEST(pq_pop_test);

    GREATEST_MAIN_END(); /* display results */
}