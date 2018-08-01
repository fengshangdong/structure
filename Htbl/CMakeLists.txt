#指定生成目标
add_executable(chtbl_test test1.c chtbl.c)
target_link_libraries(chtbl_test list_base)

add_executable(ohtbl_test test2.c ohtbl.c)
target_link_libraries(ohtbl_test list_base)
