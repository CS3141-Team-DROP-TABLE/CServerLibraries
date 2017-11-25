cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")


project(clibs)

enable_testing()

include_directories(include)

add_subdirectory(tests)
add_library(linkedList linkedList.c)
add_library(ping ping.c)
add_library(AVL AVL.c)
add_library(stringMap stringMap.c)
target_link_libraries(stringMap AVL)
add_library(configLoader configLoader.c)
target_link_libraries(configLoader AVL stringMap)
add_library(serverConnect serverConnect.c)
target_link_libraries(serverConnect gnutls)
