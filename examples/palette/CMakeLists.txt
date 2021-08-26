cmake_minimum_required(VERSION 3.12)
project(palette)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(palette
	src/main.cpp
)

set (source "${CMAKE_CURRENT_SOURCE_DIR}/content")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/content")

add_custom_command(
	TARGET palette POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
	COMMENT "copy content"
)


add_custom_command(
	TARGET palette POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../../engine/libs/openal-soft-bin/OpenAL32.dll" "${CMAKE_CURRENT_BINARY_DIR}/OpenAL32.dll"
	COMMENT "copy openal"
)


target_include_directories(palette PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(palette pxl)
