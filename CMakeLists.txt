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

include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/asio-1.22.1/include)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# include(GoogleTest)

# file(GLOB test_SRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/test/*/[!.]*.cc")
# file(GLOB test_SRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/*/[!.]*.cc")
# set(TEST_SOURCES ${test_SRC})

# set(TEST_MAIN ${PROJECT_SOURCE_DIR}/tests_main.cc)

# add_executable(tests_main ${TEST_MAIN} ${TEST_SOURCES})
# gtest_discover_tests(tests_main)

# Including custom cmake rules
# include(cmake/clang-cxx-dev-tools.cmake)

# target_link_libraries(tests_main gtest gmock gtest_main pthread)
set(COMPILE_OPTIONS
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

# target_compile_options(tests_main PRIVATE ${COMPILE_OPTIONS})

# add_test(NAME tests_main COMMAND tests_main)

file(GLOB exec_SRC CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/[!.]*.cc")
set(EXEC_SOURCES ${exec_SRC})

set(CLIENT ${PROJECT_SOURCE_DIR}/sample_client.cc)
add_executable(sample_client ${CLIENT} ${EXEC_SOURCES})

target_link_libraries(sample_client pthread)
target_compile_options(sample_client PRIVATE ${COMPILE_OPTIONS})

set(ADMIN ${PROJECT_SOURCE_DIR}/sample_admin.cc)
add_executable(sample_admin ${ADMIN} ${EXEC_SOURCES})

target_link_libraries(sample_admin pthread)
target_compile_options(sample_admin PRIVATE ${COMPILE_OPTIONS})

set(SERVER ${PROJECT_SOURCE_DIR}/sample_experiment.cc)
add_executable(sample_server ${SERVER} ${EXEC_SOURCES})

target_link_libraries(sample_server pthread)
target_compile_options(sample_server PRIVATE ${COMPILE_OPTIONS})
