#指定生成目标
add_executable(bitree_test test1.c bitree.c traverse.c)
target_link_libraries(bitree_test list_base)

add_executable(bistree_test test2.c bitree.c traverse.c bistree.c)
target_link_libraries(bistree_test list_base)
