cmake_minimum_required(VERSION 3.10)

find_package(OpenGL REQUIRED)


add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
set(OPENGL_LIBRARY
	${OPENGL_LIBRARY}
	-lGL -lGLU -lXrandr -lXext -lX11 -lrt
	${CMAKE_DL_LIBS}
	${GLFW_LIBRARIES}
)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


### GLFW ###

add_subdirectory (glfw-3.3.4)

include_directories(
	glfw-3.3.4/include/GLFW/
	glew-2.2.0/include/
)

### GLEW ###

set(GLEW_SOURCE
	glew-2.2.0/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_220 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_220
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)
