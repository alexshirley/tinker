project(ring_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        ring_test.c
        non_blocking_ring_suite.c
        dsp_ring_suite.c
        blocking_ring_suite.c
        ../../src/non_blocking_ring.c
        ../../src/dsp_ring.c
        ../../src/blocking_ring.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})