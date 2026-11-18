cmake_minimum_required(VERSION 3.14)
project(Lab3_CppTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги для сбора покрытия кода (gcov / lcov)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -g -O0")

include(FetchContent)

# Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# Google Benchmark
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.8.3
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

# Все 7 структур данных
set(STRUCTURES_SRC
    cpp_files/DynamicArray.cpp
    cpp_files/SinglyList.cpp
    cpp_files/DoublyList.cpp
    cpp_files/Stack.cpp
    cpp_files/Queue.cpp
    cpp_files/HashTable.cpp
    cpp_files/ComleteTree.cpp
)

# Единый тест на все структуры
add_executable(run_tests tests/test_all.cpp ${STRUCTURES_SRC})
target_link_libraries(run_tests gtest_main gcov)

# Бенчмарки
add_executable(run_benchmarks benchmarks/bench_all.cpp ${STRUCTURES_SRC})
target_link_libraries(run_benchmarks benchmark::benchmark benchmark::benchmark_main)