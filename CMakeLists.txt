cmake_minimum_required(VERSION 3.1)
project(maxflow-lib)

set(CMAKE_CXX_STANDARD 11)

set(MAXFLOWLIB_SRC src/)
set(LIB_SRCS ${MAXFLOWLIB_SRC}/algorithms/bk/maxflow.cpp ${MAXFLOWLIB_SRC}/algorithms/bk/graph.cpp)
set(LIB_HEADERS ${MAXFLOWLIB_SRC}/maxflow.h ${MAXFLOWLIB_SRC}/maxflow_bk.h ${MAXFLOWLIB_SRC}/algorithms/bk/block.h ${MAXFLOWLIB_SRC}/algorithms/bk/graph.h)

set(MAXFLOWLIB_HEADERS ${MAXFLOWLIB_SRC}/maxflow.h ${MAXFLOWLIB_SRC}/maxflow_bk.h)
set(BK_EXE_SRCS examples/maxflow_bk_example.cpp)

add_library(maxflow SHARED ${LIB_SRCS} ${LIB_HEADERS})

# Compile the BK example
add_executable(maxflow_bk_example ${BK_EXE_SRCS})
target_include_directories(maxflow_bk_example PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(maxflow_bk_example maxflow)
