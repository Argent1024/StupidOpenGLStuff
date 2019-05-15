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
		assert(std::abs(glm::length(isect - center) - radius) < 0.1f);

		// Get Fv on mass center
		// and Torque
		glm::vec3 dirv = glm::normalize(isect - center);
		Fv = glm::dot(dirv, F) * dirv;
		Torque = glm::cross(isect - center, F);
		//std::cout<<glm::to_string(Torque)<<std::endl;
	}

	glm::mat3 IbodyInv() {
		return glm::mat3(5.0f / (2.0f * mass * radius * radius));
	}

	void update(glm::mat3& rotation, glm::vec3& transation) {
		center = transation;
	}

	void normalAt(const glm::vec3& p, glm::vec3& normal) {
		//std::cout<<glm::to_string(p)<<" "<<glm::to_string(center)<<std::endl;
		//std::cout<<glm::length(p-center)<<" "<<radius<<std::endl;
		//TODO ???
		assert(std::abs(glm::length(p - center) - radius) < 0.1f);
		normal = glm::normalize(p - center);
	}

};

class Plane : public BoundingVolume {
	// Plane centered at center,
	// p on plane satisff p - c = xu + yv and -maxu <= x <= maxu, -maxv <= y <= maxv
public:
	glm::vec3 uorigin;
	glm::vec3 vorigin;
	glm::vec3 norigin;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 n;
	const float maxu;
	const float maxv;

	void getCoor(const glm::vec3& p, float& x, float& y) {
		glm::vec3 d = p - center;
		//assert(std::abs(glm::dot(d, n)) < 0.001f);
		x = glm::dot(p, u);
		y = glm::dot(p ,v);
		//assert(-maxu <= x && x <= maxu);
		//assert(-maxv <= y && y <= maxv);
	}

	Plane(float m, glm::vec3& c, glm::vec3& u, glm::vec3& v, float maxu, float maxv)
		:BoundingVolume(m, c), uorigin(u), vorigin(v), norigin(glm::cross(u, v)), maxu(maxu), maxv(maxv)
	{
		this->u = uorigin;
		this->v = vorigin;
		this->n = norigin;
		std::cout<<glm::to_string(this->v)<<std::endl;
		assert(std::abs(glm::length(this->u) - 1) < 0.001f);
		assert(std::abs(glm::length(this->v) - 1) < 0.001f);
		assert(maxu > 0 && maxv > 0);
	}
	
	void applyForce(const glm::vec3& p, const glm::vec3& F, 
					   		glm::vec3& Fv, glm::vec3& Torque) {
		float x, y;
		getCoor(p, x, y);	
		glm::vec3 dirv = glm::normalize(p - center);
		Fv = glm::dot(dirv, F) * dirv;
		Torque = glm::cross(p - center, F);
	}

	glm::mat3 IbodyInv() {
		//TODO 
		return glm::mat3(1.f);
	}

	void normalAt(const glm::vec3& p, glm::vec3& normal) {
		float x ,y;
		getCoor(p, x, y);
		normal = n;
	}

	void update(glm::mat3& rotation, glm::vec3& transation) {
		center = transation;
		u = rotation * uorigin;
		v = rotation * vorigin;
		n = rotation * norigin;
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
		//TODO modify this procedure
		const shared_ptr<Sphere> s1 = std::dynamic_pointer_cast<Sphere>(bv1);
		const shared_ptr<Sphere> s2 = std::dynamic_pointer_cast<Sphere>(bv2);
		const shared_ptr<Plane> plane1 = std::dynamic_pointer_cast<Plane>(bv1);
		const shared_ptr<Plane> plane2 = std::dynamic_pointer_cast<Plane>(bv2);
		
		if(s1 && s2) {
			return intersect(s1, s2);
		} else {
			if(s1 && plane2) {
				return intersect(s1, plane2);
			} else if(s2 && plane1) {
				return intersect(s2, plane1);
			}
		}
		std::cout<<"ERROR in type"<<std::endl;
		return false;
	}

	static void intersectPoint(const shared_ptr<BoundingVolume>& bv1, const shared_ptr<BoundingVolume>& bv2,
							   glm::vec3& p1, glm::vec3& p2) {
		const shared_ptr<Sphere> s1 = std::dynamic_pointer_cast<Sphere>(bv1);
		const shared_ptr<Sphere> s2 = std::dynamic_pointer_cast<Sphere>(bv2);
		const shared_ptr<Plane> plane1 = std::dynamic_pointer_cast<Plane>(bv1);
		const shared_ptr<Plane> plane2 = std::dynamic_pointer_cast<Plane>(bv2);
		
		if(s1 && s2) {
			return intersectPoint(s1, s2, p1, p2);
		} else {
			if(s1 && plane2) {
				return intersectPoint(s1, plane2, p1, p2);
			} else if(s2 && plane1) {
				return intersectPoint(s2, plane1, p1, p2);
			}
		}
		std::cout<<"ERROR in type"<<std::endl;
		return;

	}

	static bool intersect(const shared_ptr<Sphere>& s1, const shared_ptr<Sphere>& s2) {
		const glm::vec3& c1 = s1->center;
		const glm::vec3& c2 = s2->center;
		return glm::length(c1 - c2) < s1->radius + s2->radius;
	}

	static bool intersect(const shared_ptr<Sphere>& s, const shared_ptr<Plane>& p) {
		const glm::vec3& c1 = s->center;
		const glm::vec3& c2 = p->center;
		const glm::vec3& n = p->n;
		float distance = std::abs(glm::dot(c1 - c2, n));
		if(distance > s->radius) return false;
		float x = std::abs(glm::dot(c1 - c2, p->u));
		float y = std::abs(glm::dot(c1 - c2, p->v));
		return x < p->maxu && y < p->maxv;
	}
	
	static void intersectPoint(const shared_ptr<Sphere>& s1, const shared_ptr<Sphere>& s2,
							   glm::vec3& p1, glm::vec3& p2) {
		const glm::vec3& c1 = s1->center;
        const glm::vec3& c2 = s2->center;
		glm::vec3 dir = glm::normalize(c1 - c2);

		float r1 = s1->radius;
		float r2 = s2->radius;

		p1 = c1 - r1 * dir ;
        p2 = c2 + r2 * dir ;
	}

	static void intersectPoint(const shared_ptr<Sphere>& s, const shared_ptr<Plane>& p, 
							   glm::vec3& p1, glm::vec3& p2) {
	    const glm::vec3& c1 = s->center;
        const glm::vec3& c2 = p->center;

		float x = glm::dot(c1 - c2, p->u);
		float y = glm::dot(c1 - c2, p->v);
		// make sure p2 will be on the plane
		x = x < p->maxu ? x : p->maxu;
		x = x > -p->maxu ? x : p->maxu;

		y = y < p->maxv ? y : p->maxv;
		y = y > -p->maxv ? y : p->maxv;

		p2 = c2 + x * p->u + y * p->v;

		glm::vec3 dir = glm::normalize(c1 - p2);
		p1 = c1 - s->radius * dir;
	}
};



#endif