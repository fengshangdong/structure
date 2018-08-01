#生成静态库
add_library(list_base list.c clist.c dlist.c)

#执行文件
add_executable(list_test1 test1.c list.c)
add_executable(list_test2 test2.c clist.c)
add_executable(list_test3 test3.c dlist.c)
