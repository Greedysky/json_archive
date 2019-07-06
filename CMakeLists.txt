cmake_minimum_required(VERSION 2.8)
project(json_archive)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Boost 1.54.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_definitions(-DDEBUG=1 -DBOOST_PP_VARIADICS=1)
endif()

set(LIB_SRC json_iarchive_impl.cpp json_oarchive_impl.cpp json_archive.cpp)
add_library(${PROJECT_NAME} STATIC ${LIB_SRC})

install(TARGETS json_archive ARCHIVE DESTINATION lib)
file(GLOB HEADERS "./*.hpp")
install(FILES ${HEADERS} DESTINATION include)

add_executable(${PROJECT_NAME}_test test.cpp)
target_link_libraries(${PROJECT_NAME}_test json_archive boost_serialization)

