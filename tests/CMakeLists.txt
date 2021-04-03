set(test_sources
    memory_chunk_test.cpp
    memory_pool_test.cpp
    memory_pool_allocator_test.cpp)

add_executable(test ${test_sources})

target_compile_features(
   test
   PUBLIC
   cxx_std_17
)

find_package(GTest REQUIRED)

target_link_libraries(
  test
  PUBLIC
  GTest::GTest
  GTest::Main
  memory_pool::memory_pool
)