#ifndef PARTICLESYS_H
#define PARTICLESYS_H

#define PS_Node_PTR std::shared_ptr<SPNode<int>> 

#include <vector>
#include <cmath>
#include <algorithm>

#include <glm/glm.hpp>  
#include <glm/ext.hpp>

#include "bvh.h"
#include "spTree.h"

#include "physic.h"
#include "physhape.h"

#include "shape.h"

/*
	Particle System using SPH method
*/

class ParticleState: public PhysicState {
/*
	State for one particle
	physhape points to the shape of this particle, e.x. sphere
*/
public:
	float pressure;
	float density;
	glm::vec3 velocity;

	ParticleState(glm::vec3 transition, std::shared_ptr<PhyShape> physhape, 
				  float density, float pressure) 
		: PhysicState(glm::mat3(1.f), transition, physhape), 
		  density(density) , pressure(pressure) {}

	// Force field
	void updateForce(const glm::vec3& F) {
		velocity += DT * F / physhape->mass;
	}

	void updateForce(const glm::vec3& p, const glm::vec3& F) {
		updateForce(F);
	}

	void update(float dt) {
		if (glm::length(velocity) <= 0.001f) {
			return;
		}
		transition += dt * velocity;
		this->physhape->update(transition);
	}

	void getVelocityAt(const glm::vec3& p, glm::vec3& v) const {
		//std::cout << "calling getVelocityAt for particle state" << std::endl;
		v = velocity;
	}

	glm::vec3 getv() {
		return velocity;
	}

	void setShader(std::shared_ptr<Shader>& shader) {
		shader->setVec3("transition", transition);
	}
};

class ParticleSystem  {
public:
	std::vector<std::shared_ptr<ParticleState>> particles;
	std::vector<PS_Node_PTR> nodeList;
	//BoxBV bv;
	float r;

	ParticleSystem() {}

	// TODO create n instead of n^3
	// Assume box.lenx == box.leny == box.lenz for now
	void init(int n, float mass,  BoxBV& bv, std::shared_ptr<ParticleSystem> s) {
		// Given total mass, init volume, create particle system with n^3 praricles
		//this->bv = bv;
		float volume = bv.volume();
		assert(volume > 0 && mass > 0 && n > 0);

		float radius = bv.lenx / float(n) * 0.5f;
		this->r = radius;

		float m_particle = mass / volume;

		float density = m_particle / (4.f / 3.f * 3.14f * powf(radius, 3));
		float pressure = 1.f;

		const glm::vec3 start = bv.center - bv.x * bv.lenx - bv.y * bv.leny - bv.z * bv.lenz;
		const float len = bv.lenx;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					glm::vec3 pos = start + bv.x * len * i / n + bv.y * len * j / n + bv.z * len * k / n;
					std::shared_ptr<PhyShape> shape = std::make_shared<PhySphere>(radius, mass, pos);
					particles.push_back(std::make_shared<ParticleState>(pos, shape, density, pressure));
				}
			}
		}
	}
	
	bool insideBV(std::shared_ptr<ParticleState>& p, PS_Node_PTR& bv) {
		glm::vec3 dir = p->transition - bv->center;
		
		float x = std::abs(glm::dot(dir, bv->x));
		float y = std::abs(glm::dot(dir, bv->y));
		float z = std::abs(glm::dot(dir, bv->z));
		return x < bv->lenx && y < bv->leny  && z < bv->lenz;
	}
	
	// Helper method for createRoot
	void getMax(glm::vec3& max, const glm::vec3& p, int d);
	void getMin(glm::vec3& max, const glm::vec3& p, int d);

	PS_Node_PTR ParticleSystem::createRoot();

	PS_Node_PTR createSPTree(int height);

	virtual void updatePressureDensity();

	virtual void update();
	
};

// Render different ball for now, change later
class ParticleShape : public Shape {
public:
	TriMesh mesh; // Just render a ball for now
	std::shared_ptr<ParticleSystem> particleSys;

	ParticleShape(std::shared_ptr<ParticleSystem> particleSys, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		:mesh(vertices, indices), particleSys(particleSys) {
		//BallHelper::initVertices(10, radius, vertices, indices);
	}

	void render(std::shared_ptr<Shader>& shader, unsigned int& texture) {
		for (auto const & p : particleSys->particles) {
			p->setShader(shader);
			mesh.render(shader, texture);
		}
	}

};



#endif // PARTICLESYS_H
