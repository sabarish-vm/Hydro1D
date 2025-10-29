################################################################################
# This file is part of HydroCodeSpherical1D
# Copyright (C) 2017 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
#
# HydroCodeSpherical1D is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# HydroCodeSpherical1D is distributed in the hope that it will be useful,
# but WITOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with HydroCodeSpherical1D. If not, see <http://www.gnu.org/licenses/>.
################################################################################

cmake_minimum_required(VERSION 3.15)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
project(HydroCodeSpherical1D)

set(SOURCES
    main_spherical.cpp
)

configure_file(${PROJECT_SOURCE_DIR}/Parameters.hpp.in
               ${PROJECT_BINARY_DIR}/Parameters.hpp @ONLY)

include_directories(${PROJECT_BINARY_DIR})

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
else(OPENMP_FOUND)
    message(ERROR "OpenMP not found!")
endif(OPENMP_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -ftree-vectorize")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wshadow")

add_executable(HydroCodeSpherical1D ${SOURCES})
target_include_directories(HydroCodeSpherical1D PRIVATE ${PROJECT_SOURCE_DIR})
