#指定生成目标
add_executable(set_test test.c set.c)
target_link_libraries(set_test list_base)
