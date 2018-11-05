cmake_minimum_required(VERSION 3.5)
set(CTEST_PROJECT_NAME "Drafter")

set(CTEST_SOURCE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
set(CTEST_BINARY_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build")

set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

set(CTEST_USE_LAUNCHERS 1)

set(CTEST_COVERAGE_COMMAND "/usr/bin/gcov")
set(CTEST_MEMORYCHECK_COMMAND "/usr/bin/valgrind")

ctest_start("Continuous")
ctest_configure(OPTIONS "-DCMAKE_CXX_FLAGS=--coverage -DCMAKE_BUILD_TYPE=Debug")
ctest_build(FLAGS -j5)
ctest_test()
ctest_coverage(CAPTURE_CMAKE_ERROR COVERAGE_ERROR)
ctest_memcheck()
ctest_submit()
