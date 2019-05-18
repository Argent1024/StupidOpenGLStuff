#include "physhape.h"

void Collision::intersectP(std::shared_ptr<PhyShape>& s1,  std::shared_ptr<PhyShape>& s2,
         std::shared_ptr<PhysicState>& phy1,  std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2){
    //TODO modify this procedure
	std::shared_ptr<PhySphere> sphere1 = std::dynamic_pointer_cast<PhySphere>(s1);
    std::shared_ptr<PhySphere> sphere2 = std::dynamic_pointer_cast<PhySphere>(s2);
    std::shared_ptr<PhyPlane> plane1 = std::dynamic_pointer_cast<PhyPlane>(s1);
    std::shared_ptr<PhyPlane> plane2 = std::dynamic_pointer_cast<PhyPlane>(s2);
		
	if(sphere1 && sphere2) {
        return Collision::intersectP(sphere1, sphere2, phy1, phy2, p1, p2);
    } else {
        if(sphere1 && plane2) {
            return Collision::intersectP(sphere1, plane2,phy1, phy2, p1, p2);
        } else if(sphere2 && plane1) {
            return Collision::intersectP(sphere2, plane1,phy1, phy2, p1, p2);
        }
    }
	std::cout<<"ERROR in type"<<std::endl;
	return;
}

// Intersection point for two sphere
void Collision::intersectP(std::shared_ptr<PhySphere>& s1, std::shared_ptr<PhySphere>& s2,
        std::shared_ptr<PhysicState>& phy1, std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2) {
    glm::vec3 ndir = glm::normalize(s1->center - s2->center);
    p1.p = s1->center - s1->radius * ndir;
    p2.p = s2->center + s2->radius * ndir;
    phy1->getVelocityAt(p1.p, p1.v);
    phy2->getVelocityAt(p2.p, p2.v);
    p1.n = -ndir;
    p2.n = ndir;

    glm::vec3 vrel = p1.v - p2.v; 
    // move contact point a little bit to simulate deformation
    if(std::abs(glm::dot(vrel, ndir)) >= 0.01f) {
        // Recalculate contact point and velocity but don't calculate normal again
        glm::vec3 vdir = glm::normalize(vrel);
        glm::vec3 pdir1 = glm::normalize(s1->center - vdir);
        glm::vec3 pdir2 = glm::normalize(s2->center + vdir);
        p1.p = s1->center - s1->radius * pdir1;
        p2.p = s2->center - s2->radius * pdir2;
        phy1->getVelocityAt(p1.p, p1.v);
        phy2->getVelocityAt(p2.p, p2.v);
    }
}

void Collision::intersectP(std::shared_ptr<PhySphere>& sphere, std::shared_ptr<PhyPlane>& plane,
        std::shared_ptr<PhysicState>& phy1, std::shared_ptr<PhysicState>& phy2,
        ContactPoint& p1, ContactPoint& p2) {
    float x = glm::dot(sphere->center - plane->center, plane->u);
	float y = glm::dot(sphere->center - plane->center, plane->v);
	// make sure p2 will be on the plane
	x = x <  plane->maxu ? x : plane->maxu;
	x = x > -plane->maxu ? x : plane->maxu;

	y = y <  plane->maxv ? y : plane->maxv;
	y = y > -plane->maxv ? y : plane->maxv;
    p2.p = plane->center + x * plane->u + y * plane->v;
    glm::vec3 dir = glm::normalize(sphere->center - p2.p);
	p1.p = sphere->center - sphere->radius * dir;
    phy1->getVelocityAt(p1.p, p1.v);
    phy2->getVelocityAt(p2.p, p2.v);

    glm::vec3 vrel = p1.v - p2.v; 
    // move contact point a little bit to simulate deformation
    if(std::abs(glm::dot(vrel, dir)) >= 0.01f) {
        // Recalculate contact point and velocity but don't calculate normal again
        glm::vec3 vdir = glm::normalize(vrel);
        glm::vec3 pdir1 = glm::normalize(sphere->center - vdir);
        p1.p = sphere->center - sphere->radius * pdir1;
        phy1->getVelocityAt(p1.p, p1.v);
    }
}

