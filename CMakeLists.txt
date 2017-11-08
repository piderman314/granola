cmake_minimum_required(VERSION 3.0)

project(granola)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_compile_options(/utf-8 /std:c++17 /W4 /WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-std=c++1z -stdlib=libc++ -Wall -Wextra)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(-std=c++1z -Wall -Wextra)
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

file(GLOB granola_SRC "include/*.hpp" "src/*.cpp" "include/tokenizer/*.hpp" "src/tokenizer/*.cpp")

if (test)

	if(cppcheck)

		list(APPEND CPPCHECK_CMAKE_ARGS
			"-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
		)

		ExternalProject_Add(
			cppcheck
			GIT_REPOSITORY      https://github.com/danmar/cppcheck.git
			GIT_TAG             1.81
			CMAKE_ARGS          ${CPPCHECK_CMAKE_ARGS}
			PREFIX              ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
			TMP_DIR             ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
			STAMP_DIR           ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
			DOWNLOAD_DIR        ${CMAKE_BINARY_DIR}/external/cppcheck/download
			SOURCE_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/src
			BINARY_DIR          ${CMAKE_BINARY_DIR}/external/cppcheck/build
		)

		list(APPEND CPPCHECK_ARGS
			--enable=warning,style,performance,portability,unusedFunction
			--std=c++11
			--verbose
			--error-exitcode=1
			--language=c++
			-DMAIN=main
			-I ${CMAKE_SOURCE_DIR}/include
			${CMAKE_SOURCE_DIR}/include/*.hpp
			${CMAKE_SOURCE_DIR}/include/tokenizer/*.hpp
			${CMAKE_SOURCE_DIR}/src/*.cpp
			${CMAKE_SOURCE_DIR}/src/src/*.hpp
			${CMAKE_SOURCE_DIR}/doctest/*.cpp
		)

		add_custom_target(
			check
			COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS}
			COMMENT "running cppcheck"
		)

	endif()

	
	if(asan)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
	endif()

	if(usan)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
	endif()

	if(tsan)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
	endif()

	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --leak-check=full")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --track-fds=yes")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --trace-children=yes")
	set(MEMORYCHECK_COMMAND_OPTIONS "${MEMORYCHECK_COMMAND_OPTIONS} --error-exitcode=1")

	include(CTest)

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