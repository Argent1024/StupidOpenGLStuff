#ifndef BVH_H
#define BVH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Oriented Bounding Boxes
class BoundingVolume {
public:
	float mass;
	BoundingVolume(float m):mass(m){} 
	// Calculate how force F apply at isect point
	// F always points into bv ...?
	virtual void applyForce(const glm::vec3& isect, const glm::vec3& F, 
					   		glm::vec3& Fv, glm::vec3& Torque) = 0;
	
	virtual glm::mat3 IbodyInv() = 0;
};

class Sphere : public BoundingVolume{
public:
	float radius;
	glm::vec3 center;
	
	Sphere(float r):BoundingVolume(1.f), radius(r), center(0.f, 0.f,0.f) {}

	void applyForce(const glm::vec3& isect, const glm::vec3& F, 
					glm::vec3& Fv, glm::vec3& Torque) {
		// Get Fv on mass center
		// and Torque
		glm::vec3 dirv = glm::normalize(isect - center);
		Fv = glm::dot(dirv, F) * dirv;
		Torque = glm::cross(isect - center, F);
	}

	glm::mat3 IbodyInv() {
		return glm::mat3(2.0f * mass * radius * radius / 5.0f);
	}

};

class Plane {
	//TODO

};

class Box {
	//TODO
};

class BoundingVolumeTree {
	//TODO
};

class Collision {
public:
	static bool intersect(Sphere& s1, Sphere& s2) {
		glm::vec3& c1 = s1.center;
		glm::vec3& c2 = s2.center;
		return glm::length(c1 - c2) < c1.r + c2.r;
	}
};

#endif