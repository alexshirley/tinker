project(priority_queue_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        priority_queue_test.c
        ../../src/priority_queue.c
        ../../src/cvector.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})