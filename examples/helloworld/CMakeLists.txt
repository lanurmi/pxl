cmake_minimum_required(VERSION 3.12)
project(helloworld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(helloworld
	src/main.cpp
)

add_custom_command(
	TARGET helloworld POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/content" "${CMAKE_CURRENT_BINARY_DIR}/content"
	COMMENT "copy content"
)

add_custom_command(
	TARGET helloworld POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../../engine/libs/openal-soft-bin/OpenAL32.dll" "${CMAKE_CURRENT_BINARY_DIR}/OpenAL32.dll"
	COMMENT "copy openal"
)

target_include_directories(helloworld PUBLIC  ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(helloworld pxl)
