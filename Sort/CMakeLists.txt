set(sort_SRCS
	issort.c 
	qksort.c 
	mgsort.c 
	ctsort.c 
	rxsort.c 
	bisearch.c
)

#生成静态库,链接math库
add_library(sort_base ${sort_SRCS})
target_link_libraries(sort_base m)

#指定生成目标
add_executable(sort_test test.c)
target_link_libraries(sort_test sort_base)
