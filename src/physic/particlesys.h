#ifndef PARTICLESYS_H
#define PARTICLESYS_H

#include <vector>

#include <glm/glm.hpp>  
#include <glm/ext.hpp>

#include "bvh.h"

#include "physic.h"
#include "physhape.h"

#include "shape.h"

/*
	Particle System using SPH method
*/

class ParticleState: public PhysicState{
/*
	State for one particle
	physhape points to the shape of this particle, e.x. sphere
*/
private:
	glm::vec3 velocity;

protected:
	float density;
	glm::vec3 pressure;

public:
	ParticleState(glm::vec3 transition, std::shared_ptr<PhyShape> physhape, 
				  float density, glm::vec3 pressure) 
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

		float radius = bv.lenx / n / 2.f;
		this->r = radius;

		float m_particle = mass / volume;

		float density = m_particle / (4.f / 3.f * 3.14f * powf(radius, 3));
		glm::vec3 pressure(0.f);

		const glm::vec3 start = bv.center - bv.x * bv.lenx - bv.y * bv.leny - bv.z * bv.lenz;
		const float len = bv.lenx;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				for (int k = 0; k < n; k++) {
					glm::vec3 pos = start + bv.x * len * i / n + bv.y * len * j / n + bv.z * len * k / n;
					std::shared_ptr<PhyShape> shape = std::make_shared<PhySphere>(radius, m_particle, pos);
					particles.push_back(std::make_shared<ParticleState>(pos, shape, density, pressure));
				}
			}
		}
	}

	virtual void update() {
		return;
	}
	
	//Smoothing kernels
	virtual float Wpoly(float r, float h) {
		return 0;
	}


};


// Render different ball for now, change later
class ParticleShape : public Shape {
public:
	TriMesh mesh;
	std::shared_ptr<ParticleSystem> particleSys;

	ParticleShape(std::shared_ptr<ParticleSystem> particleSys, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
		:mesh(vertices, indices), particleSys(particleSys){
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
