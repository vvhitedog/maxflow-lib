cmake_minimum_required(VERSION 3.1)
project(maxflow-lib)

set(CMAKE_CXX_STANDARD 11)

set(LIB_SRCS src/algorithms/bk/maxflow.cpp src/algorithms/bk/graph.cpp)
set(LIB_HEADERS src/maxflow.h src/maxflow_bk.h src/algorithms/bk/block.h src/algorithms/bk/graph.h)

set(EXE_SRCS src/maxflow_bk_test.cpp)

add_library(maxflow SHARED ${LIB_SRCS} ${LIB_HEADERS})

add_executable(maxflow_bk_test ${EXE_SRCS})
target_link_libraries(maxflow_bk_test maxflow)
