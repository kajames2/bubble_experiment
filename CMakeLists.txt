cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -DBOOST_MATH_OVERFLOW_ERROR_POLICY=ignore_error")
# set(CMAKE_CXX_CLANG_TIDY
#   clang-tidy;
#   -header-filter='^src';
#   -checks=*,-llvmlibc-*,-modernize-use-trailing-return-type,-fuchsia-*;)

project(asset)

include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(GoogleTest)

file(GLOB test_SRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/test/*/[!.]*.cc")
file(GLOB test_SRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*/[!.]*.cc")
set(TEST_SOURCES ${test_SRC})

set(TEST_MAIN ${PROJECT_SOURCE_DIR}/tests_main.cc)

add_executable(tests_main ${TEST_MAIN} ${TEST_SOURCES})
gtest_discover_tests(tests_main)

# Including custom cmake rules
# include(cmake/clang-cxx-dev-tools.cmake)

target_link_libraries(tests_main gtest gmock gtest_main pthread)
target_compile_options(tests_main PRIVATE
  -W
  -Wall
  -Wextra
  -Wshadow
  -Wnon-virtual-dtor
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wpedantic
  -Wconversion
  -Wsign-conversion
  -Wdouble-promotion
  -Wformat=2
  -pedantic)
add_test(NAME tests_main COMMAND tests_main)
