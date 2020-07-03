project(fir_example LANGUAGES C)

add_executable(${PROJECT_NAME}
        fir_example.c
        ../../src/dsp_ring.c
)

add_test(${PROJECT_NAME} ${PROJECT_NAME})