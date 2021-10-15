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
	
	src/backbuffer.cpp

	src/containers/string.cpp


	src/backends/filesystem_backend_cpp.cpp
	src/backends/gamepad.cpp
	src/backends/keyboard.cpp
	src/backends/mouse.cpp
	src/backends/graphics_backend_gl.cpp
	src/backends/platform_backend_sdl.cpp
	src/backends/platform_backend_dummy_win32.cpp
	src/backends/audio_backend_openal.cpp
	src/backends/audio_backend_dummy.cpp
	src/backends/imageproc_backend_cpp.cpp
	src/backends/video_backend_ffmpeg.cpp
	src/backends/video_backend_dummy.cpp

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
	src/assets/image.cpp
	src/assets/imageformats.cpp
	src/assets/font.cpp
	src/assets/audio.cpp
	src/assets/audioformats.cpp
	
	src/math/calc.cpp
	src/math/color.cpp
	src/math/mat3x2.cpp
	src/math/mat4x4.cpp
	src/math/rect.cpp
	src/math/vec2.cpp
	src/math/vec3.cpp	
	src/math/tween.cpp
	src/math/grid.cpp

	src/utils/input_binding.cpp
	
	
	src/utils/image_packer.cpp
	src/utils/filestream.cpp

	src/ec/entity.cpp
	src/ec/component.cpp
	src/ec/world.cpp
	src/ec/component_interface.cpp
)

target_include_directories(pxl
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

set(PXL_PLATFORM_SDL true CACHE BOOL "Use SDL2 Platform Backend")
set(PXL_GRAPHICS_OPENGL true CACHE BOOL "Use OpenGL Graphics Backend")
set(PXL_AUDIO_OPENAL true CACHE BOOL "Use OpenAL audio Backend")
set(PXL_AUDIO_DUMMY false CACHE BOOL "Use OpenAL audio Backend")
set(PXL_PLATFORM_DUMMY_WIN32 false CACHE BOOL "Use Dummy WIN32 Platform Backend")
set(PXL_STL_CONTAINERS true CACHE BOOL "Use STL containers")
set(PXL_VIDEO_FFMPEG false CACHE BOOL "Use FFMPEG video Backend")
set(PXL_VIDEO_DUMMY true CACHE BOOL "Use Dummy video Backend")

if (PXL_STL_CONTAINERS)
	target_compile_definitions(pxl PUBLIC PXL_USE_STL_CONTAINERS)
endif()

if (PXL_PLATFORM_DUMMY_WIN32)
    target_compile_definitions(pxl PUBLIC PXL_PLATFORM_DUMMY_WIN32)
endif()

if (PXL_GRAPHICS_OPENGL)
	target_compile_definitions(pxl PUBLIC PXL_GRAPHICS_OPENGL)
endif()

set(LIBS "")

if (PXL_PLATFORM_SDL)
	set(SDL2_PATH "libs/SDL")
	target_compile_definitions(pxl PUBLIC PXL_PLATFORM_SDL)
	add_subdirectory( ${SDL2_PATH} )
	set(LIBS ${LIBS} SDL2main SDL2-static)
	target_include_directories(pxl PUBLIC ${SDL2_PATH}/include)
endif()

if (PXL_AUDIO_OPENAL)
	set(OPENAL_PATH "libs/openal-soft")
	target_compile_definitions(pxl PUBLIC PXL_AUDIO_OPENAL)
	add_subdirectory( ${OPENAL_PATH} )
	target_include_directories(pxl PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/libs/openal-soft/include")
	set(LIBS ${LIBS} ${CMAKE_CURRENT_BINARY_DIR}/libs/openal-soft/OpenAL32.lib)
endif()

if (PXL_VIDEO_FFMPEG)

    target_compile_definitions(pxl PUBLIC PXL_VIDEO_FFMPEG)
    INCLUDE_DIRECTORIES("${PXL_FFMPEG_DIR}/include")
    LINK_DIRECTORIES("${PXL_FFMPEG_DIR}/lib64-14.0")

    set(LIBS ${LIBS}
        ${PXL_FFMPEG_DIR}/lib64-14.0/avcodec.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/avdevice.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/avfilter.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/avformat.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/avutil.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/swresample.lib
        ${PXL_FFMPEG_DIR}/lib64-14.0/swscale.lib
    )
endif()

target_link_libraries(pxl PUBLIC ${LIBS})
