#ifndef PHYWORLD_H
#define PHYWORLD_H

#include "physic.h"
#include "physhape.h"
#include "particlesys.h"

class PhysicWorld {
public:
    //TODO Use BVH
    std::vector<std::shared_ptr<PhysicState>> phyobjs;
	std::vector<std::shared_ptr<ParticleSystem>> particleSystem;

    void push_back(std::shared_ptr<PhysicState> phy) {
        phyobjs.push_back(phy);
    }

    void updateForceField() {
		// Add force fields on every objs
        glm::vec3 G(0.f, -1.f, 0.f);
        for(auto const & phy : phyobjs) {
            phy->updateForce(G);
        }
    }

	void update(float dt) {
		for (auto const & ps : particleSystem) {
			ps->update();
		}

		//Go dt time
		for (auto const & phy : phyobjs) {
			phy->update(dt);
		}
	}

	void addParticleSystem(std::shared_ptr<ParticleSystem> ps) {
		for (auto const & p_state : ps->particles) {
			phyobjs.push_back(p_state);
		}
		particleSystem.push_back(ps);
	}

    // Calculation collision, update data in each phystate
    void collisionDetection() {
        for(auto& phy1 : phyobjs) {
            for(auto& phy2 : phyobjs) {
                
                if (phy1 == phy2 || !Collision::intersect(phy1->physhape, phy2->physhape)) continue;
                // collision happen
                // std::cout<< "collision happen!!!!!"<<std::endl;
                // TODO go back some time to when collision happen
                phy1->update(-1.f / 120.f);
                phy2->update(-1.f / 120.f);

                // calculate intersection point and normal
                ContactPoint p1, p2;
                //TODO stupid const....
                Collision::intersectP(phy1->physhape, phy2->physhape, phy1, phy2, p1, p2);
                // calculate force
                const float m1 = phy1->physhape->mass;
                const float m2 = phy2->physhape->mass;

                
				if (glm::length(p1.v - p2.v) < 0.01f) { 
					phy1->update(1.f / 120.f);
					phy2->update(1.f / 120.f);
					continue; 
				}

                glm::vec3 Vrel1 = glm::dot(p1.v - p2.v, p1.n) * p1.n;
                glm::vec3 Vrel2 = glm::dot(p2.v - p1.v, p2.n) * p2.n;
                

				// TODO modify constant 
                float c = (-62.5f / (1.f/m1 + 1.f/m2));
                
				glm::vec3 F1 = c * Vrel1;
                glm::vec3 F2 = c * Vrel2;

                phy1->updateForce(p1.p, F1);
                phy2->updateForce(p2.p, F2);

                phy1->update(1.f / 120.f);
                phy2->update(1.f / 120.f);
            }
        }
    }
};
extern PhysicWorld GamePhysic;

#endif