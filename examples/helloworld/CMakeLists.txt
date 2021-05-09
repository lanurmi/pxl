cmake_minimum_required(VERSION 3.12)
project(helloworld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(helloworld
	src/main.cpp
)

set (source "${CMAKE_CURRENT_SOURCE_DIR}/content")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/content")

add_custom_command(
	TARGET helloworld POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
	COMMENT "copy content"
)

target_include_directories(helloworld PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(helloworld pxl)
