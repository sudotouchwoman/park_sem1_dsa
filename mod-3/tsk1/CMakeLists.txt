cmake_minimum_required(VERSION 3.16)
project(Graphs)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED)
add_compile_options(-Wall -Werror -Wpedantic)

set(PROJECT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

add_library(IGraph STATIC 
src/ArcGraph.cpp
src/ListGraph.cpp
src/MatrixGraph.cpp
src/SetGraph.cpp
)
target_include_directories(IGraph PUBLIC inc/)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC IGraph)