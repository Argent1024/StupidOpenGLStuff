#include "game.h"
#include "loader.h"

// init resorce manger
ShaderManager GameShaderManger;
TextureManager GameTexManger;
ShapeManager GameShapeManger;
Camera GameCamera;

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
    window = glfwCreateWindow(720, 720, "Playground", NULL, NULL);

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

    Vertex e(
        glm::vec3(0.0f, 0.0f, -0.5f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(0.5f, 0.5f) 
    );

    vector<Vertex> v = {a, b, c, d, e};
    vector<unsigned int> indices = {
        0, 1, 3, 
        1, 2, 3,
        4, 0, 1,
        4, 0, 3,
        4, 1, 2,
        4, 2, 3
    };
    
    string path = "/home/argent/work/game/src/";
	//string path = "D:/work/game/src/";
    string vp =  path +"shaders/testvertex.glsl";
    string fp =  path +"shaders/testfragment.glsl";
    //string texp = path + "textures/wall.jpg";
    //string texp = path + "textures/2k_earth_repeat.jpg";
    //string texp = path + "textures/2k_jupiter.jpg";
    string texp = path + "textures/PoolBallSkins/Ball9.jpg";
    string texname = "testtex";
    string shadername = "testshader";
    GameShaderManger.load(shadername, vp, fp);
    GameTexManger.load(texname, texp);

    string shapename = "testMesh";
    vector<Vertex> b_vertex;
    vector<unsigned int> b_indices;
    
    BallHelper::initVertices(100, 0.5f, b_vertex, b_indices);

    GameShapeManger.load(shapename, b_vertex, b_indices);
    shared_ptr<BoundingVolume> bv = make_shared<Sphere>(0.5);

    GameObject* testObj = new GameObject(shadername, shapename, bv, texname, RIGIDBODY);
    this->objs.push_back(testObj);
    return 0;
}

int Game::run() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetKeyCallback(window, Game::key_callback);
    do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

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

