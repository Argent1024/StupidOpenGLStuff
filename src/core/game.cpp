#include "game.h"
#include "loader.h"

// init resorce manger
ShaderManager GameShaderManger;
TextureManager GameTexManger;
ShapeManager GameShapeManger;
Camera GameCamera;
PhysicWorld GamePhysic;

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
    glm::mat3 R(1.f);
    Vertex a(
        glm::vec3(10.f, -0.5f, 10.f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(1.0f, 1.0f) 
    );

    Vertex b(
        glm::vec3(10.f, -0.5f, -10.f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(1.0f, 0.0f) 
    );

    Vertex c(
        glm::vec3(-10.f, -0.5f, -10.f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(0.0f, 0.0f) 
    );

    Vertex d(
        glm::vec3(-10.f, -0.5f, 10.f),
        glm::vec3(0.0f, 0.0f, 1.0f),  
        glm::vec2(0.0f, 1.0f) 
    );

    vector<Vertex> vertices = {a, b, c, d};
    vector<unsigned int> indices = {
        0, 1, 3, 
        1, 2, 3,
    };
    
    string path = "/home/argent/work/game/src/";
	//string path = "D:/work/game/src/";
    string vp =  path +"shaders/testvertex.glsl";
    string fp =  path +"shaders/testfragment.glsl";
    string shadername = "testshader";
    GameShaderManger.load(shadername, vp, fp);

    /*Ground*/
    string wall = "Wall";
    string walltexp = path + "textures/2k_earth_daymap.jpg";
    GameTexManger.load(wall, walltexp);
    glm::vec3 u(1.f, 0.f, 0.f);
    glm::vec3 v(0.f, 0.f, 1.f);
    glm::vec3 c0(0.f, -0.5f, 0.f);

    shared_ptr<PhyShape> groundbv = make_shared<PhyPlane>(1.0, c0, u, v, 10.f, 10.f);
    GameShapeManger.load("Ground", vertices, indices);
    GameObj* obj0 = createObj(shadername,"Ground", groundbv, R, c0, wall, NOPHYSIC);

    string texp = path + "textures/PoolBallSkins/";
    string ball1 = "Ball1.jpg";
    GameTexManger.load(ball1, texp + ball1);

    string ball2 = "Ball2.jpg";
    GameTexManger.load(ball2, texp + ball2);

    string ball3 = "Ball3.jpg";
    GameTexManger.load(ball3, texp + ball3);

    string shapename = "testMesh";
    vector<Vertex> b_vertex;
    vector<unsigned int> b_indices;
    BallHelper::initVertices(40, 0.2f, b_vertex, b_indices);
    GameShapeManger.load(shapename, b_vertex, b_indices);

    glm::vec3 c1(1.f, 1.0f, 1.f);
    glm::vec3 c2(-1.f, 1.0f, 0.8f);
    glm::vec3 c3(1.f, 1.f, -0.15f);
    glm::vec3 c4(0.f, 0.5f, 0.f);
    glm::vec3 c5(0.f, 0.5f, 1.f);
    glm::vec3 c6(0.7f, 0.5f, 1.f);
    vector<glm::vec3> ballpos = {c1, c2, c3, c4, c5, c6};
    vector<GameObj*> objlist;
    for(int i = 1; i <= 6; i++) {
        glm::vec3 p = ballpos[i-1];
        string ballname = "Ball" + to_string(i) + ".jpg";
        GameTexManger.load(ballname, texp + ballname);
        shared_ptr<PhySphere> bv = make_shared<PhySphere>(0.2f, 1.f, p);
        GameObj* obj = createObj(shadername, shapename, bv, R, p, ballname, RIGIDBODY);
        objlist.push_back(obj);
    }
    objlist[0]->applyForce(glm::vec3(0.8f, 1.0f, 1.0f), glm::vec3(-1.f, 0.f, 1.f));
    objlist[1]->applyForce(glm::vec3(-1.2f, 1.0f, 0.8f), glm::vec3(1.f, 0.0f, 0.f));
    objlist[2]->applyForce(glm::vec3(1.2f, 1.f, -0.15f), glm::vec3(-1.f, 0.f, 1.f));
    objlist[5]->applyForce(glm::vec3(0.7f, 0.5, 1.2f), glm::vec3(-0.f, 2.f, 1.f));
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

        GamePhysic.updateForceField();
        GamePhysic.collisionDetection();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
           glfwWindowShouldClose(window) == 0 );
    glfwTerminate();
    return 0;
}

