cmake_minimum_required(VERSION 3.0)

project(granola)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/utf-8 /std:c++17)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-std=c++1z)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(-std=c++1z)
endif()

include(ExternalProject)
	
ExternalProject_Add(
	doctest
	PREFIX ${CMAKE_BINARY_DIR}/doctest
	GIT_REPOSITORY https://github.com/onqtam/doctest.git
	GIT_TAG 1.2.5
	TIMEOUT 10
	CONFIGURE_COMMAND ""
	BUILD_COMMAND ""
	INSTALL_COMMAND ""
	LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(doctest source_dir)
set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")

file(GLOB granola_SRC "include/*.h" "src/*.cpp" "src/*.c")

if (test)
	enable_testing()

	add_executable(granola_test ${granola_SRC} "doctest/run_doctest.cpp")
	add_dependencies(granola_test doctest)

	target_include_directories(granola_test PRIVATE include)
	target_include_directories(granola_test PRIVATE ${DOCTEST_INCLUDE_DIR})

	add_test(granola_test granola_test)
else()
	add_library(granola ${granola_SRC} "doctest/do_not_run_doctest.cpp")
	target_include_directories(granola PUBLIC include)

	target_include_directories(granola PRIVATE ${DOCTEST_INCLUDE_DIR})
endif()