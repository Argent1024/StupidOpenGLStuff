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
/*
// Oriented Bounding Boxes
class BoundingVolume {
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
				return intersectPoint(s2, plane1, p2, p1);
			}
		}
		std::cout<<"ERROR in type"<<std::endl;
		return;

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

*/

#endif