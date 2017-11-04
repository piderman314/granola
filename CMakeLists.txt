cmake_minimum_required(VERSION 3.0)

project(granola)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/utf-8 /std:c++17)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-std=c++1z)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(-std=c++1z)
endif()

file(GLOB granola_SRC "include/*.h" "src/*.cpp" "src/*.c")

add_library(granola ${granola_SRC})

target_include_directories(granola PUBLIC include)

if (test)
	find_package(GTest REQUIRED)

	enable_testing()

	add_executable(granola_test "test/granola_test.cpp")
	target_include_directories(granola_test PRIVATE ${GTEST_INCLUDE_DIR})
	target_link_libraries(granola_test PRIVATE ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY})

	add_test(granola_test granola_test)
endif()