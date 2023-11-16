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

if (WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(GLEW_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.lib)
	else()
		set(GLEW_LIBRARIES ${CMAKE_CURRENT_LIST_DIR}/libs/glew-2.1.0/lib/Release/Win32/glew32.lib)
	endif()
	set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/submodules/glew/include)
else()
	set(glew-cmake_BUILD_STATIC ON)
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/submodules/glew)
	set(GLEW_LIBRARIES libglew_static)
endif()

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_LOWERCASE)
if (CMAKE_BUILD_TYPE_LOWERCASE STREQUAL "debug")
	set(GctkDebug 1)
else()
	set(GctkDebug 0)
endif()

file(GLOB GCTK_SRC "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB GCTK_LUA_BINDING_SRC "${CMAKE_CURRENT_LIST_DIR}/src/lua_bindings/*.c")

add_library(gctk SHARED
	${GCTK_SRC}
	${GCTK_LUA_BINDING_SRC}
)
target_compile_definitions(gctk PRIVATE GCTK_DLL_EXPORT)
target_compile_definitions(gctk PUBLIC GCTK_DEBUG=${GctkDebug})
target_include_directories(gctk PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${GLEW_INCLUDE_DIRS})
target_link_libraries(gctk PUBLIC lua_static glfw ${GLEW_LIBRARIES} soil2 OpenGL::GL cglm_headers)

add_executable(gctk_test test/main.c)
target_link_libraries(gctk_test PRIVATE gctk)