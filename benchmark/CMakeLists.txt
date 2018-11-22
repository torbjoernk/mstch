add_executable(mstch_benchmark
    ${CMAKE_CURRENT_LIST_DIR}/benchmark_main.cpp)
target_link_libraries(mstch_benchmark
    PUBLIC mstch
           benchmark)
