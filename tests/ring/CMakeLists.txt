project(ring_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        ring_test.c
        non_blocking_ring_suite.c
        ../../src/non_blocking_ring.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})