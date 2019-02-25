
cmake_minimum_required(VERSION 3.7)

project(SSE_Profiles)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "-std=c++14")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mavx2")

find_package(Boost 1.61.0 REQUIRED  system filesystem timer coroutine log  fiber COMPONENTS)
ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

add_executable( driver driver.cxx )
target_link_libraries(driver )
target_link_libraries(driver ${Boost_LIBRARIES} pthread)
