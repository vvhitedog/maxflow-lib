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
cmake_minimum_required(VERSION 3.9)

set(PROJECT_VERSION 1.0.0)

project(maxflow-lib VERSION ${PROJECT_VERSION} DESCRIPTION "Performant maxflow algorithms under common interface.")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

# Setup maxflow-library
set(MAXFLOWLIB_SRC src/)
set(BK_SRCS ${MAXFLOWLIB_SRC}/algorithms/bk/maxflow.cpp ${MAXFLOWLIB_SRC}/algorithms/bk/graph.cpp )
set(IBFS_SRCS ${MAXFLOWLIB_SRC}/algorithms/ibfs/ibfs.cpp)
set(LIB_SRCS ${BK_SRCS} ${IBFS_SRCS} )
set(MAXFLOWLIB_HEADERS ${MAXFLOWLIB_SRC}/maxflow.h ${MAXFLOWLIB_SRC}/maxflow_bk.h)
set(LIB_HEADERS ${MAXFLOWLIB_HEADERS} ${MAXFLOWLIB_SRC}/algorithms/bk/block.h ${MAXFLOWLIB_SRC}/algorithms/bk/graph.h)
add_library(maxflow SHARED ${LIB_SRCS} ${LIB_HEADERS})
target_include_directories(maxflow PRIVATE ${MAXFLOWLIB_SRC})
# Install setup:
set_target_properties(maxflow PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
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
