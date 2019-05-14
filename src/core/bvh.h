#ifndef BVH_H
#define BVH_H

#include "assert.h"
#include <cmath>
#include <iostream>
#include <typeinfo>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Oriented Bounding Boxes
class BoundingVolume {
public:
	float mass;
	glm::vec3 center;

	BoundingVolume(float m, glm::vec3& c):mass(m), center(c){} 

	// Calculate how force F apply at isect point
	// Put result in Fv & Torque
	virtual void applyForce(const glm::vec3& isect, const glm::vec3& F, 
					   		glm::vec3& Fv, glm::vec3& Torque) = 0;
	
	// Apply force on mass center
	virtual void applyForce(const glm::vec3& F, glm::vec3& Fv) {
		Fv = F;
	}

	virtual glm::mat3 IbodyInv() = 0;

	virtual void normalAt(const glm::vec3& p, glm::vec3& normal) = 0;

	virtual void update(glm::mat3& rotation, glm::vec3& transation) = 0;
};

class Sphere : public BoundingVolume{
public:
	float radius;
	Sphere(float r, glm::vec3& c):BoundingVolume(1.f, c), radius(r) {}

	void applyForce(const glm::vec3& isect, const glm::vec3& F, 
					glm::vec3& Fv, glm::vec3& Torque) {
		// Make sure isect is on(near) sphere
		assert(std::abs(glm::length(isect - center) - radius) < 0.0001f);

		// Get Fv on mass center
		// and Torque
		glm::vec3 dirv = glm::normalize(isect - center);
		Fv = glm::dot(dirv, F) * dirv;
		Torque = 100.f * glm::cross(isect - center, F);
		//std::cout<<glm::to_string(Torque)<<std::endl;
	}

	glm::mat3 IbodyInv() {
		return glm::mat3(2.0f * mass * radius * radius / 5.0f);
	}

	void update(glm::mat3& rotation, glm::vec3& transation) {
		center = transation;
	}

	void normalAt(const glm::vec3& p, glm::vec3& normal) {
		assert(std::abs(glm::length(p - center) - radius) < 0.0001f);
		normal = glm::normalize(p - center);
	}

};

class Box {
	//TODO
};

class BoundingVolumeTree {
	//TODO
};

class Collision {
public:
	static bool intersect(const shared_ptr<BoundingVolume>& bv1, const shared_ptr<BoundingVolume>& bv2) {
		//std::cout<<"ERROR calling this function"<<std::endl;
		const shared_ptr<Sphere> s1 = std::dynamic_pointer_cast<Sphere>(bv1);
		const shared_ptr<Sphere> s2 = std::dynamic_pointer_cast<Sphere>(bv2);
		if(!s1 || !s2) {
			std::cout<<"error in bv type"<<std::endl;
			return false;
		}
		return intersect(s1, s2);
	}

	static bool intersect(const shared_ptr<Sphere>& s1, const shared_ptr<Sphere>& s2) {
		const glm::vec3& c1 = s1->center;
		const glm::vec3& c2 = s2->center;
		return glm::length(c1 - c2) < s1->radius + s2->radius;
	}
};



#endif