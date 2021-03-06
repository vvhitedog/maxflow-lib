#
#  This file is part of maxflow-lib.
#
#  maxflow-lib is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  maxflow-lib is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with maxflow-lib.  If not, see <https://www.gnu.org/licenses/>.
#
# @file CMakeLists.txt
#
# @brief CMake build config.
#
# @author Matt Gara
#
# @date 2019-04-14
#
#
cmake_minimum_required(VERSION 2.8)

project(maxflow-lib)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} )
set(PROJECT_AUTHOR Matt Gara )
set(PROJECT_AUTHOR_EMAIL gara.matt@gmail.com )
set(PROJECT_DESCRIPTION  "Performant maxflow algorithms under common interface.")

include(GNUInstallDirs)

# Set standard to c++11
if (${CMAKE_VERSION} VERSION_GREATER 3.0)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
else ()
  ADD_DEFINITIONS( -std=c++11 )
endif ()

# Setup maxflow-library
set(MAXFLOWLIB_SRC src/)
set(BK_SRCS ${MAXFLOWLIB_SRC}/algorithms/bk/maxflow.cpp ${MAXFLOWLIB_SRC}/algorithms/bk/graph.cpp)
set(IBFS_SRCS ${MAXFLOWLIB_SRC}/algorithms/ibfs/ibfs.cpp)
set(HPF_SRCS ${MAXFLOWLIB_SRC}/algorithms/hpf/pseudo.cpp)
set(UTIL_SRCS ${MAXFLOWLIB_SRC}/util/timer.cpp)
set(LIB_SRCS ${BK_SRCS} ${IBFS_SRCS} ${HPF_SRCS})
set(MAXFLOWLIB_HEADERS ${MAXFLOWLIB_SRC}/maxflow.h ${MAXFLOWLIB_SRC}/maxflow_bk.h ${MAXFLOWLIB_SRC}/maxflow_ibfs.h ${MAXFLOWLIB_SRC}/maxflow_hpf.h)
set(LIB_HEADERS ${MAXFLOWLIB_HEADERS} ${MAXFLOWLIB_SRC}/algorithms/bk/block.h ${MAXFLOWLIB_SRC}/algorithms/bk/graph.h)
add_library(maxflow SHARED ${LIB_SRCS} ${LIB_HEADERS})
target_include_directories(maxflow PRIVATE ${MAXFLOWLIB_SRC})
# Install setup:
set_target_properties(maxflow PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION ${PROJECT_VERSION_MAJOR}
  PUBLIC_HEADER "${MAXFLOWLIB_HEADERS}")
install(TARGETS maxflow
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/maxflowlib )

# Compile the BK example
set(BK_EXE_SRCS examples/maxflow_bk_example.cpp)
add_executable(maxflow_bk_example ${BK_EXE_SRCS})
target_include_directories(maxflow_bk_example PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(maxflow_bk_example maxflow)

# Compile the IBFS example
set(IBFS_EXE_SRCS examples/maxflow_ibfs_example.cpp)
add_executable(maxflow_ibfs_example ${IBFS_EXE_SRCS})
target_include_directories(maxflow_ibfs_example PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(maxflow_ibfs_example maxflow)

# Compile the HPF example
set(HPF_EXE_SRCS examples/maxflow_hpf_example.cpp)
add_executable(maxflow_hpf_example ${HPF_EXE_SRCS})
target_include_directories(maxflow_hpf_example PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(maxflow_hpf_example maxflow)

# Compile the DIMACS benchmark example
set(BENCHMARK_EXE_SRCS examples/maxflow_benchmark_dimacs.cpp ${UTIL_SRCS})
add_executable(maxflow_benchmark_dimacs ${BENCHMARK_EXE_SRCS})
target_include_directories(maxflow_benchmark_dimacs PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(maxflow_benchmark_dimacs maxflow)

# Compile the create_wrapped_gaussian binary
set(PU_GAUSS_EXE_SRCS examples/phase_unwrapping/create_wrapped_gaussian.cpp)
add_executable(create_wrapped_gaussian ${PU_GAUSS_EXE_SRCS})
target_include_directories(create_wrapped_gaussian PRIVATE ${MAXFLOWLIB_SRC})

# Compile the py_visualize_gaussian binary
set(PU_VIS_GAUSS_EXE_SRCS examples/phase_unwrapping/py_visualize_gaussian.cpp)
add_executable(py_visualize_gaussian ${PU_VIS_GAUSS_EXE_SRCS})
target_include_directories(py_visualize_gaussian PRIVATE ${MAXFLOWLIB_SRC})

# Compile the py_visualize_gaussian binary
set(PU_UNW_GAUSS_EXE_SRCS examples/phase_unwrapping/unwrap_gaussian.cpp ${UTIL_SRCS})
add_executable(unwrap_gaussian ${PU_UNW_GAUSS_EXE_SRCS})
target_include_directories(unwrap_gaussian PRIVATE ${MAXFLOWLIB_SRC})
target_link_libraries(unwrap_gaussian maxflow)
