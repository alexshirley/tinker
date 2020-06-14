#include <greatest.h>


TEST dummy_test(){
    PASS();
}

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_TEST(dummy_test);

    GREATEST_MAIN_END(); /* display results */
}