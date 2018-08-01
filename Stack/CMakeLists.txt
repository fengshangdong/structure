#指定生成目标
add_executable(stack_test test1.c stack.c)
target_link_libraries(stack_test list_base)

add_executable(queue_test test2.c queue.c)
target_link_libraries(queue_test list_base)
