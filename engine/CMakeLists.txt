cmake_minimum_required(VERSION 3.12)
project(pxl)

# C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("$<$<CONFIG:DEBUG>:-DPXLDEBUG>")

add_library(pxl

	src/engine.cpp
	src/config.cpp
	src/time.cpp
	src/log.cpp
	src/types.cpp
	src/filesystem.cpp

	src/platforms/platform_backend_sdl.cpp
		
	src/graphics/batch.cpp
	src/graphics/blend.cpp
	src/graphics/graphics_backend_gl.cpp
	src/graphics/material.cpp
	src/graphics/mesh.cpp
	src/graphics/render_target.cpp
	src/graphics/sampler.cpp
	src/graphics/shader.cpp
	src/graphics/texture.cpp
	src/graphics/sprite_font.cpp
	src/graphics/Subtexture.cpp
	
	src/image/image.cpp
	src/image/font.cpp
	
	src/input/gamepad_backend.cpp
	src/input/keyboard_backend.cpp
	src/input/mouse_backend.cpp
	
	src/math/calc.cpp
	src/math/color.cpp
	src/math/mat3x2.cpp
	src/math/mat4x4.cpp
	src/math/rect.cpp
	src/math/vec2.cpp
	src/math/vec3.cpp	
	
	
	src/utils/input_binding.cpp
	src/utils/scene.cpp
	src/utils/entity.cpp
	src/utils/component.cpp
	src/utils/scene_manager.cpp
	src/utils/image_packer.cpp
 )

target_include_directories(pxl
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set(PLATFORM_SDL2 true CACHE BOOL "Use SDL2 Platform Backend")
set(GRAPHICS_OPENGL true CACHE BOOL "Use OpenGL Graphics Backend")

set(LIBS "")

if (GRAPHICS_OPENGL)
	add_compile_definitions(PXL_GRAPHICS_OPENGL)
endif()

if (PLATFORM_SDL2)
	add_compile_definitions(PXL_PLATFORM_SDL2)

	# Try to find SDL2
	if (DEFINED SDL2_LIBRARIES AND DEFINED SDL2_INCLUDE_DIRS)
		set(SDL2_FOUND true)

	else()
		find_package(SDL2 QUIET)

	endif()	

	# If CMake cannot find SDL2 library, then it gets downloaded and compiled that way
	if (NOT ${SDL2_FOUND})
		include(FetchContent)
		FetchContent_Declare(
			SDL2
			GIT_REPOSITORY https://github.com/libsdl-org/SDL
			GIT_TAG release-2.0.14 # grab latest stable release
		)
		FetchContent_GetProperties(SDL2)

		if (NOT sdl2_POPULATED)
			FetchContent_Populate(SDL2)
			add_subdirectory(${sdl2_SOURCE_DIR} ${sdl2_BINARY_DIR})

		endif()

		# statically link SDL2 since we're building it ourselves
		set(LIBS ${LIBS} SDL2main SDL2-static)
		target_include_directories(pxl PUBLIC ${sdl2_SOURCE_DIRS}/include)

	else()
		# Add Library and Include Dirs
		set(LIBS ${LIBS} ${SDL2_LIBRARIES})
		target_include_directories(pxl PUBLIC ${SDL2_INCLUDE_DIRS})

	endif()

endif()

target_link_libraries(pxl PUBLIC ${LIBS})