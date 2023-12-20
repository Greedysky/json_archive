# ***************************************************************************
# * Copyright (C) 2015 - 2024 Greedysky Studio
#
# * Use, modification and distribution is subject to the Boost Software
# * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# * http://www.boost.org/LICENSE_1_0.txt).
# ***************************************************************************

cmake_minimum_required(VERSION 3.0.0)

project(json_archive)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost 1.64.0 REQUIRED)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-DDEBUG=1 -DBOOST_PP_VARIADICS=1)
endif()

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_library(${PROJECT_NAME} STATIC json_iarchive_impl.cpp json_oarchive_impl.cpp json_archive.cpp)
add_executable(${PROJECT_NAME}_demo main.cpp)
target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME} boost_serialization)

install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)
file(GLOB INSTALL_HEADERS "./*.hpp")
install(FILES ${INSTALL_HEADERS} DESTINATION include)
