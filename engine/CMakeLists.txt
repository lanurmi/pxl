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
	
	src/containers/string.cpp
	src/containers/vector.cpp
	src/containers/map.cpp
	src/containers/array.cpp


	src/backends/filesystem_backend_cpp.cpp
	src/backends/gamepad.cpp
	src/backends/keyboard.cpp
	src/backends/mouse.cpp
	src/backends/graphics_backend_gl.cpp
	src/backends/platform_backend_sdl.cpp
	src/backends/audio_backend_openal.cpp
	src/backends/audio_backend_dummy.cpp

	src/graphics/batch.cpp
	src/graphics/blend.cpp
	src/graphics/material.cpp
	src/graphics/mesh.cpp
	src/graphics/rendertarget.cpp
	src/graphics/sampler.cpp
	src/graphics/shader.cpp
	src/graphics/texture.cpp
	src/graphics/subtexture.cpp
	src/graphics/atlas.cpp
	src/graphics/sprite.cpp
	src/graphics/tileset.cpp
	src/graphics/spritefont.cpp

	src/audio/audiosource.cpp
	src/audio/audiobuffer.cpp
	
	src/assets/aseprite.cpp
	src/assets/ogmo.cpp
	src/assets/image.cpp
	src/assets/imageformats.cpp
	src/assets/ldtk.cpp
	src/assets/font.cpp
	src/assets/audio.cpp
	
	src/math/calc.cpp
	src/math/color.cpp
	src/math/mat3x2.cpp
	src/math/mat4x4.cpp
	src/math/rect.cpp
	src/math/vec2.cpp
	src/math/vec3.cpp	
	src/math/tween.cpp
	
	src/utils/input_binding.cpp
	src/utils/scene.cpp
	src/utils/entity.cpp
	src/utils/component.cpp
	src/utils/scene_manager.cpp
	src/utils/image_packer.cpp
	src/utils/filestream.cpp

	src/utils/components/camera.cpp
)

target_include_directories(pxl
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set(PLATFORM_SDL2 true CACHE BOOL "Use SDL2 Platform Backend")
set(GRAPHICS_OPENGL true CACHE BOOL "Use OpenGL Graphics Backend")
set(AUDIO_OPENAL true CACHE BOOL "Use OpenAL audio Backend")
set(AUDIO_DUMMY false CACHE BOOL "Use OpenAL audio Backend")

if (GRAPHICS_OPENGL)
	add_compile_definitions(PXL_GRAPHICS_OPENGL)
endif()

set(LIBS "")

if (PLATFORM_SDL2)
	set(SDL2_PATH "libs/SDL")
	add_compile_definitions(PXL_PLATFORM_SDL)
	add_subdirectory( ${SDL2_PATH} )
	set(LIBS ${LIBS} SDL2main SDL2-static)
	target_include_directories(pxl PUBLIC ${SDL2_PATH}/include)
endif()

if (AUDIO_OPENAL)
	set(OPENAL_PATH "libs/openal-soft")
	add_compile_definitions(PXL_AUDIO_OPENAL)
	add_subdirectory( ${OPENAL_PATH} )
	target_include_directories(pxl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/include")
	set(LIBS ${LIBS} ${CMAKE_CURRENT_BINARY_DIR}/libs/openal-soft/OpenAL32.lib)
endif()

target_link_libraries(pxl PUBLIC ${LIBS})