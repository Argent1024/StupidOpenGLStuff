add_subdirectory (lib)

include_directories(
	lib/glfw-3.1.2/include/
	lib/glm-0.9.7.1/
    lib/glew-1.13.0/include/
    core/
    ./
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
)

file( GLOB GAME_SRC
    "core/*.hpp"
    "core/*.cpp"
)


add_executable(Gmain
	main.cpp
	${GAME_SRC}
)

target_link_libraries(Gmain 
    ${ALL_LIBS}
)