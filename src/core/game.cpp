#include "game.h"
#include "loader.h"

// init resorce manger
ShaderManager GameShaderManger;
TextureManager GameTexManger;
ShapeManager GameShapeManger;
Camera GameCamera;
PhysicWorld GamePhysic;

void Game::test_scene() {
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

	std::vector<Vertex> vertices = { a, b, c, d };
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3,
	};

	//string path = "/home/argent/work/game/src/";
	std::string path = "D:/work/game/src/";
	std::string vp = path + "shaders/testvertex.glsl";
	std::string fp = path + "shaders/testfragment.glsl";
	std::string shadername = "testshader";
	GameShaderManger.load(shadername, vp, fp);

	/*Ground*/
	std::string wall = "Wall";
	std::string walltexp = path + "textures/2k_earth_daymap.jpg";
	GameTexManger.load(wall, walltexp);
	glm::vec3 u(1.f, 0.f, 0.f);
	glm::vec3 v(0.f, 0.f, 1.f);
	glm::vec3 c0(0.f, -0.5f, 0.f);

	std::shared_ptr<PhyShape> groundbv = std::make_shared<PhyPlane>(1.0, c0, u, v, 10.f, 10.f);
	GameShapeManger.load("Ground", vertices, indices);
	GameObj* obj0 = createObj(shadername, "Ground", groundbv, R, c0, wall, NOPHYSIC);

	std::string texp = path + "textures/PoolBallSkins/";
	std::string ball1 = "Ball1.jpg";
	GameTexManger.load(ball1, texp + ball1);

	std::string ball2 = "Ball2.jpg";
	GameTexManger.load(ball2, texp + ball2);

	std::string ball3 = "Ball3.jpg";
	GameTexManger.load(ball3, texp + ball3);

	std::string shapename = "testMesh";
	std::vector<Vertex> b_vertex;
	std::vector<unsigned int> b_indices;
	BallHelper::initVertices(40, 0.2f, b_vertex, b_indices);
	GameShapeManger.load(shapename, b_vertex, b_indices);

	glm::vec3 c1(1.f, 1.0f, 1.f);
	glm::vec3 c2(-1.f, 1.0f, 0.8f);
	glm::vec3 c3(1.f, 1.f, -0.15f);
	glm::vec3 c4(0.f, 0.5f, 0.f);
	glm::vec3 c5(0.f, 0.5f, 1.f);
	glm::vec3 c6(0.7f, 0.5f, 1.f);
	std::vector<glm::vec3> ballpos = { c1, c2, c3, c4, c5, c6 };
	std::vector<GameObj*> objlist;
	for (int i = 1; i <= 6; i++) {
		glm::vec3 p = ballpos[i - 1];
		std::string ballname = "Ball" + std::to_string(i) + ".jpg";
		GameTexManger.load(ballname, texp + ballname);
		std::shared_ptr<PhySphere> bv = std::make_shared<PhySphere>(0.2f, 1.f, p);
		GameObj* obj = createObj(shadername, shapename, bv, R, p, ballname, RIGIDBODY);
		objlist.push_back(obj);
	}
	objlist[0]->applyForce(glm::vec3(0.8f, 1.0f, 1.0f), glm::vec3(-1.f, 0.f, 1.f));
	objlist[1]->applyForce(glm::vec3(-1.2f, 1.0f, 0.8f), glm::vec3(1.f, 0.0f, 0.f));
	objlist[2]->applyForce(glm::vec3(1.2f, 1.f, -0.15f), glm::vec3(-1.f, 0.f, 1.f));
	objlist[5]->applyForce(glm::vec3(0.7f, 0.5, 1.2f), glm::vec3(-0.f, 2.f, 1.f));
}

void Game::test_praticle() {
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

	std::vector<Vertex> vertices = { a, b, c, d };
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3,
	};

	//string path = "/home/argent/work/game/src/";
	std::string path = "D:/work/game/src/";
	std::string vp = path + "shaders/testvertex.glsl";
	std::string fp = path + "shaders/testfragment.glsl";
	std::string shadername = "testshader";
	GameShaderManger.load(shadername, vp, fp);

	/*Ground*/
	std::string wall = "Wall";
	std::string walltexp = path + "textures/2k_earth_daymap.jpg";
	GameTexManger.load(wall, walltexp);
	glm::vec3 u(1.f, 0.f, 0.f);
	glm::vec3 v(0.f, 0.f, 1.f);
	glm::vec3 c0(0.f, -0.5f, 0.f);

	std::shared_ptr<PhyShape> groundbv = std::make_shared<PhyPlane>(1.0, c0, u, v, 10.f, 10.f);
	GameShapeManger.load("Ground", vertices, indices);
	GameObj* obj0 = createObj(shadername, "Ground", groundbv, R, c0, wall, NOPHYSIC);
	
	/*praticle tex*/
	std::string p_texpath = path + "textures/PoolBallSkins/Ball2.jpg";
	std::string p_texname = "PraticeTEX";
	GameTexManger.load(p_texname, p_texpath);
	createPraticleSys(shadername, p_texname);
}

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
	test_praticle();
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
		GamePhysic.update(1 / 30.f);
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


