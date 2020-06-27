#include <priority_queue.h>
#include <greatest.h>

TEST pass_test(){
    PASS();
}

int pq_comp_min(const void * a, const void * b){
    int* val1 = (int*)a;
    int* val2 = (int*)b;
    return (val1 < val2) - (val2 > val1);
}

TEST pq_creation_test(){
    char buf[1000];
    prio_q_t* hdl;
    ASSERT_EQ(true, prio_q_init(&hdl,buf,1000,sizeof(int),pq_comp_min));
    int trash;
    ASSERT_EQ(false, prio_q_peek(hdl, &trash));
    PASS();
}

TEST pq_insert_test(){

}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(pass_test);
    RUN_TEST(pq_creation_test);

    GREATEST_MAIN_END(); /* display results */
}