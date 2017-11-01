
include_directories(include)

add_library(linkedList linkedList.c)
add_library(ping ping.c)
add_library(serverConnect serverConnect.c)
target_link_libraries(serverConnect gnutls)
