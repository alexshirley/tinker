project(ring_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        ring_test.c
        ../../src/ring.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})