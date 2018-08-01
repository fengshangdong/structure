#CMake最低版本号
cmake_minimum_required (VERSION 2.8)

#项目信息
project (structure)

#编译选项
set(CXX_FLAGS
	-g
	-Wall
	-march=native
	-rdynamic
)

#支持gcc以及设置编译选项
set(CMAKE_CXX_COMPILER "gcc")
set(REPLACE CMAKE_CXX_FLAGS ${CXX_FLAGS})

#二进制执行文件和静态库文件存放位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#程序能够包含项目代码作为库头文件
include_directories(${PROJECT_SOURCE_DIR})

#添加子目录
add_subdirectory(List)
add_subdirectory(Stack)
add_subdirectory(Set)
add_subdirectory(Htbl)
add_subdirectory(Tree)
add_subdirectory(Heap)
add_subdirectory(Sort)
