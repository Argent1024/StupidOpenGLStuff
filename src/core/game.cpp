#include "game.h"
#include "loader.h"

ShaderManger GameShaderManger;
TextureManger GameTexManger;

int Game::init() {
    if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);

    if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    load();
    return 1;
}

int Game::load() {
    string path = "/home/argent/work/game/src/";
    string vp =  path +"shaders/testvertex.glsl";
    string fp =  path +"shaders/testfragment.glsl";
    string texp = path + "textures/wall.jpg";
    string tname = "testtex";
    GameShaderManger.load("testshader", vp, fp);
    GameTexManger.load(tname, texp);

    Vertex a(
        glm::vec3(0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(1.0f, 1.0f) 
    );

    Vertex b(
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(1.0f, 0.0f) 
    );

    Vertex c(
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(0.0f, 0.0f) 
    );

    Vertex d(
        glm::vec3(-0.5f, 0.5f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(0.0f, 1.0f) 
    );

    vector<Vertex> v = {a, b, c, d};
    vector<unsigned int> indices = {
        0, 1, 3, 
        1, 2, 3
    };
    TriMesh* testMesh = new TriMesh("testshader", v, indices, RIGIDBODY); 
    testMesh->setTexture(tname);
    objs.push_back(testMesh);   
    return 0;
}

int Game::run() {
    glm::mat4 T = glm::mat4(1.f);
    T = glm::rotate(T, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 1.0f));   
    do{
		glClear(GL_COLOR_BUFFER_BIT);

        // Render objects
        for(GameObj* obj : objs) {
            obj->render();
        }

        // Updates
        for(GameObj* obj : objs) {
            obj->update();
        }

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
    glfwTerminate();
    return 0;
}

