#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "loader.h"
#include "gameObject.h"
#include "shape.h"

class Game {
private:
    GLFWwindow* window;
	std::vector<GameObj*> objs;

    GameObject* createObj(const std::string& shaderName, const std::string& shapeName,
				   std::shared_ptr<PhyShape> physhape,
                   glm::mat3 rotation,
                   glm::vec3 transation,
                   const std::string& texname,
                   const PhysicType type) 
    {
        GameObject* testObj = new GameObject(shaderName, shapeName, physhape, 
                                              rotation, transation, texname, type);
        this->objs.push_back(testObj);
        return testObj;
    }

	GameObj* createParticleSys(const std::string& shaderName, const std::string& texname) {
		// num of particles
		int n = 6;
		float mass = 1.f;

		glm::vec3 c(1.f, 1.f, 0.f);
		glm::vec3 x(1.f, 0.f, 0.f);
		glm::vec3 y(0.f, 1.f, 0.f);
		glm::vec3 z(0.f, 0.f, 1.f);

		BoxBV bv(c, x, y, z, 1.f, 1.f, 1.f);
		std::shared_ptr<ParticleSystem> particlesys = std::make_shared<ParticleSystem>();
		particlesys->init(n, mass, bv, particlesys);

		int sample = 5;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		BallHelper::initVertices(sample, particlesys->r, vertices, indices);
		std::shared_ptr<Shape> p_shape = std::make_shared<ParticleShape>(particlesys, vertices, indices);

		GamePhysic.addParticleSystem(particlesys);
		ParticleObject* obj = new ParticleObject(shaderName, p_shape, texname);
		this->objs.push_back(obj);
		return obj;
	}

public:
    // initialize all stuff, load everything
    int init();
    
	// Just a test method for now, going to delete
	void test_scene();
	void test_particle();

    int load();

    // TODO hard code....
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        GameCamera.updateInput(key, action);
    }

    // Game loop
    int run();
};



#endif