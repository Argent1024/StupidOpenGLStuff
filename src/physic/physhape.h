#ifndef PHYSHAPE_H
#define PHYSHAPE_H

#include "physic.h"

struct ContactPoint {
    glm::vec3 p;
    glm::vec3 v;
    glm::vec3 n;
};

class PhySphere: public PhyShape {
public:
    float radius;
    float deformat;
    PhySphere(float r, float mass, glm::vec3 c): PhyShape(mass, c), radius(r), deformat(0.01f * r) {}

    void AssertPoint(const glm::vec3& p) {
        assert(std::abs(glm::length(p - center) - radius) <= 0.1f);
    }

    glm::mat3 getIbody() {
        return glm::mat3((2.0f * mass * radius * radius) / 5.0f);
    }

    glm::mat3 getIbodyInv() {
		return glm::mat3(5.0f / (2.0f * mass * radius * radius));
    }
};

class PhyPlane : public PhyShape {
	// Plane centered at center,
	// p on plane satisff p - c = xu + yv and -maxu <= x <= maxu, -maxv <= y <= maxv
public:
	const glm::vec3 uorigin;
	const glm::vec3 vorigin;
	const glm::vec3 norigin;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 n;
	const float maxu;
	const float maxv;

	void AssertPoint(const glm::vec3& p) {
		glm::vec3 d = p - center;
		assert(std::abs(glm::dot(d, n)) < 0.001f);
		float x = glm::dot(p, u);
		float y = glm::dot(p ,v);
		assert(-maxu <= x && x <= maxu);
		assert(-maxv <= y && y <= maxv);
	}

	PhyPlane(float m, glm::vec3& c, glm::vec3& u, glm::vec3& v, float maxu, float maxv)
		:PhyShape(m, c), uorigin(u), vorigin(v), norigin(glm::cross(u, v)), 
        maxu(maxu), maxv(maxv), u(u), v(v), n(glm::cross(u, v))
	{
		assert(std::abs(glm::length(this->u) - 1) < 0.001f);
		assert(std::abs(glm::length(this->v) - 1) < 0.001f);
		assert(maxu > 0 && maxv > 0);
	}
	
    glm::mat3 getIbody() {
		//TODO 
		return glm::mat3(1.f);
	}

	glm::mat3 getIbodyInv() {
		//TODO 
		return glm::mat3(1.f);
	}

	void update(glm::mat3& rotation, glm::vec3& transation) {
		center = transation;
		u = rotation * uorigin;
		v = rotation * vorigin;
		n = rotation * norigin;
	}
};

class Collision {
public:
	static bool intersect(std::shared_ptr<PhyShape>& s1, std::shared_ptr<PhyShape>& s2) {
		//std::cout<<"ERROR calling this function"<<std::endl;
		//TODO modify this procedure
		std::shared_ptr<PhySphere> sphere1 = std::dynamic_pointer_cast<PhySphere>(s1);
        std::shared_ptr<PhySphere> sphere2 = std::dynamic_pointer_cast<PhySphere>(s2);
        std::shared_ptr<PhyPlane> plane1 = std::dynamic_pointer_cast<PhyPlane>(s1);
        std::shared_ptr<PhyPlane> plane2 = std::dynamic_pointer_cast<PhyPlane>(s2);
		
		if(sphere1 && sphere2) {
            return intersect(sphere1, sphere2);
        } else {
            if(sphere1 && plane2) {
                return intersect(sphere1, plane2);
            } else if(sphere2 && plane1) {
                return intersect(sphere2, plane1);
            }
        }
		std::cout<<"ERROR in type"<<std::endl;
		return false;
	}

    static bool intersect(std::shared_ptr<PhySphere>& s1, std::shared_ptr<PhySphere>& s2) {
		const glm::vec3& c1 = s1->center;
		const glm::vec3& c2 = s2->center;
		return glm::length(c1 - c2) < s1->radius + s2->radius;
	}

	static bool intersect(std::shared_ptr<PhySphere>& s, std::shared_ptr<PhyPlane>& p) {
		const glm::vec3& c1 = s->center;
		const glm::vec3& c2 = p->center;
		const glm::vec3& n = p->n;
		float distance = std::abs(glm::dot(c1 - c2, n));
		if(distance > s->radius) return false;
		float x = std::abs(glm::dot(c1 - c2, p->u));
		float y = std::abs(glm::dot(c1 - c2, p->v));
		return x < p->maxu && y < p->maxv;
	}

    //TODO Fucking stupid
    static void intersectP(std::shared_ptr<PhyShape>& s1,  std::shared_ptr<PhyShape>& s2,
         std::shared_ptr<PhysicState>& phy1,  std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2);

    static void intersectP( std::shared_ptr<PhySphere>& s1,  std::shared_ptr<PhySphere>& s2,
        std::shared_ptr<PhysicState>& phy1,  std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2);

    static void intersectP( std::shared_ptr<PhySphere>& s1,  std::shared_ptr<PhyPlane>& s2,
         std::shared_ptr<PhysicState>& phy1,  std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2);
};

#endif