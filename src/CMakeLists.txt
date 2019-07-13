add_subdirectory (lib)

include_directories(
	lib/glfw-3.1.2/include/
	lib/glm-0.9.7.1/
	lib/glew-2.1.0/include/
	
	utils/
	core/
	physic/
    ./
)

file( GLOB GAME_SRC
	"utils/*.h"
    "core/*.h"
	"core/*.cpp"
	"physic/*.h"
	"physic/*.cpp"
	#"physic/particlesys.cpp"
	#"physic/physhape.cpp"
	#"physic/physic.cpp"
)


add_executable(Gmain
	main.cpp
	${GAME_SRC}
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

target_link_libraries(Gmain 
    ${ALL_LIBS}
)

if(WIN32)
	target_link_libraries(Gmain ${CMAKE_CURRENT_SOURCE_DIR}/lib/glew-2.1.0/lib/Release/x64/glew32.lib)
endif()