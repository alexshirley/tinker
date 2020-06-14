project(cvector_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        cvector_test.c
        ../../src/cvector.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})