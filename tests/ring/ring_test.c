#define _CRT_SECURE_NO_WARNINGS
#include <greatest.h>

#define NUM_ARR_ELE(x) (sizeof(x) / sizeof(x[0]))

SUITE_EXTERN(non_blocking_ring_suite);
SUITE_EXTERN(dsp_ring_suite);
SUITE_EXTERN(blocking_ring_suite);

GREATEST_MAIN_DEFS();

int main(int argc, char** argv) {
    GREATEST_MAIN_BEGIN(); /* command-line arguments, initialization. */
    /* If tests are run outside of a suite, a default suite is used. */
    RUN_SUITE(non_blocking_ring_suite);
    RUN_SUITE(dsp_ring_suite);
    RUN_SUITE(blocking_ring_suite);

    GREATEST_MAIN_END(); /* display results */
}
