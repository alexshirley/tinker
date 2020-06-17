cmake_minimum_required(VERSION 3.13)

project(tinker_tests LANGUAGES C)

include(CTest)

include_directories( 
    .
    ../ThirdParty/greatest
    ../inc/tinker
)

# At first, we want all the targets defined in the subdirectories to have the same warnings configured.
if (MSVC)
    add_compile_options(/W4 /WX)
    add_compile_options(/wd4127) # conditional expression is constant
    add_compile_options(/wd4244) # 'argument' : conversion from 'type1' to 'type2', possible loss of data
    add_compile_options(/wd4996) # Your code uses a function, class member, variable, or typedef that's marked deprecated
    add_compile_options(/wd4201) # nonstandard extension used : nameless struct/union
    add_compile_options(/wd4267) # conversion from 'size_t' to 'int', possible loss of data
    add_compile_options(/wd4204) # nonstandard extension used: non-constant aggregate initializer
    add_compile_options("/MT$<$<CONFIG:Release>:>")
else()
    add_compile_options(-g -Wall -Wextra -pedantic -pedantic-errors -Wsign-conversion -Wno-error=incompatible-pointer-types -Werror)
    add_compile_options(-Wno-error=incompatible-pointer-types)
    add_compile_options(-Wno-error=unused-parameter)
    add_compile_options(-Wno-error=unused-function)
    add_compile_options(-Wno-error=sign-compare)
    add_compile_options(-Wno-error=sign-conversion)
    add_compile_options(-Wno-error=format=)
    add_compile_options(-Wno-error=implicit-function-declaration)
    add_compile_options(-Wno-error=pedantic)
endif()

if (WIN32)
    # We target Windows 7
    add_definitions(-D_WIN32_WINNT=0x0601)
endif()

add_subdirectory(cvector)


