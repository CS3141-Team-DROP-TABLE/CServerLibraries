
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

enable_testing()
find_package(Check REQUIRED)
include_directories()
set(LIBS ${LIBS} ${CHECK_LIBRARIES} pthread linkedList AVL)

include_directories(. ../src)

add_executable(ll_test ll_test.c)
target_link_libraries(ll_test ${LIBS})
add_test(ll_test ${CMAKE_CURRENT_BINARY_DIR}/ll_test)


add_executable(AVL_test AVL_test.c)
target_link_libraries(AVL_test ${LIBS})
add_test(AVL_test ${CMAKE_CURRENT_BINARY_DIR}/AVL_test)
