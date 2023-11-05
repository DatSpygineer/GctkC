cmake_minimum_required(VERSION 3.26)
project(gctk C)
project(gctk_test C)

set(CMAKE_C_STANDARD 23)

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	if (MINGW)
		add_compile_options(-fPIC)
	endif()
else()
	add_compile_options(-fPIC)
endif()

find_package(OpenGL REQUIRED)

add_library(soil2 STATIC
	${CMAKE_CURRENT_LIST_DIR}/submodules/soil2/src/SOIL2/image_DXT.c
	${CMAKE_CURRENT_LIST_DIR}/submodules/soil2/src/SOIL2/image_helper.c
	${CMAKE_CURRENT_LIST_DIR}/submodules/soil2/src/SOIL2/SOIL2.c
	${CMAKE_CURRENT_LIST_DIR}/submodules/soil2/src/SOIL2/wfETC.c
)
target_include_directories(soil2 PUBLIC ${CMAKE_CURRENT_LIST_DIR}/submodules/soil2/src)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/submodules/glfw)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/submodules/lua)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/submodules/cglm EXCLUDE_FROM_ALL)

set(glew-cmake_BUILD_STATIC 1)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/submodules/glew)

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASE)
if (CMAKE_BUILD_TYPE_LOWERCASE STREQUAL "debug")
	set(GctkDebug 1)
else()
	set(GctkDebug 0)
endif()

add_library(gctk SHARED
	src/gctk.c
	src/gctk_math.c
	src/gctk_debug.c
	src/gctk_filesys.c
	src/gctk_string.c
		src/gctk_texture.c
		src/gctk_shader.c
		src/gctk_sprite.c
)
target_compile_definitions(gctk PRIVATE GCTK_DLL_EXPORT)
target_compile_definitions(gctk PUBLIC GCTK_DEBUG=${GctkDebug})
target_include_directories(gctk PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gctk PUBLIC lua_static glfw libglew_static soil2 OpenGL::GL cglm_headers)

add_executable(gctk_test test/main.c)
target_link_libraries(gctk_test PRIVATE gctk)