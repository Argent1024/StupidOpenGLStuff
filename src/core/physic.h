#ifndef PHYSIC_H
#define PHYSIC_H

#include "assert.h"
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "oglutil/shader.h"

#include "bvh.h"

enum PhysicType {
    NOPHYSIC,
    RIGIDBODY,
    PRATICLE
};

const float DT = 1 / 30.f;

class PhysicState {
	// protected:
public:
	// The transform
    shared_ptr<BoundingVolume> bv;
    glm::vec3 transition;
	glm::mat3 rotation;

    PhysicState(shared_ptr<BoundingVolume> bv) : bv(bv) {
		rotation = glm::mat3(1.f);
        transition = glm::vec3(0.f);
    }

    PhysicState(glm::mat3& r, glm::vec3& v, shared_ptr<BoundingVolume> bv)
        :rotation(r),transition(v),bv(bv) {}

    inline void setRotation(glm::mat3& r) {
        rotation = r;
    }

    inline void setTransition(glm::vec3& t) {
        transition = t;
    }

    virtual void updateForce(const glm::vec3& p, const glm::vec3& F) {
        return;
    }

    // Force field
    virtual void updateForce(const glm::vec3& F) {
        return;
    }

    virtual void getVelocityAt(const glm::vec3& p, glm::vec3& normal, glm::vec3& v) = 0;
    virtual void update(float dt) = 0;
    virtual void setShader(shared_ptr<Shader>& shader) = 0;
    virtual glm::vec3 getv() = 0;
};

class NoPhysic : public PhysicState {
public:
    NoPhysic(shared_ptr<BoundingVolume> bv): PhysicState(bv) {}

    NoPhysic(shared_ptr<BoundingVolume> bv, glm::mat3& r, glm::vec3& v):
        PhysicState(r, v, bv) {}

    glm::vec3 getv() {
        return glm::vec3(0.f);
    }

    void getVelocityAt(const glm::vec3& p, glm::vec3& normal, glm::vec3& v) {
        bv->normalAt(p, normal);
        v = glm::vec3(0.f);
    }

    void update(float dt) {
        return;
    }
    void setShader(shared_ptr<Shader>& shader){
        shader->setMat3("rotation", rotation);
        shader->setVec3("transition", transition);
    }
};

class RigidBody : public PhysicState {
private:
    glm::vec3 velocity;
    glm::vec3 angular;
    glm::vec3 P; // Linear Momentum
    glm::vec3 L; // Angular Momentum

    void updateRotation(float dt) {
        if(glm::length(angular) <= 0.001f) {
            return;
        }
        float x = angular.x;
        float y = angular.y;
        float z = angular.z;
        glm::mat3 crossw(0.f, -z , y,
                         z, 0.f, -x,
                         -y, x, 0.f);
        // Derivation of rotation
        glm::mat3 dR =  crossw * rotation;
        rotation += dt * dR;

        //TODO avoid numerical error
        rotation[0] = glm::normalize(rotation[0]);
        rotation[1] = glm::normalize(rotation[1]);
        rotation[2] = glm::normalize(rotation[2]);
    }

    void updateTransition(float dt) {
        if(glm::length(velocity) <= 0.001f) {
            return;
        }
        transition += dt * velocity;
    }

public:
    RigidBody(shared_ptr<BoundingVolume> bv): PhysicState(bv) {
        this->angular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->velocity = glm::vec3(0.f, 0.f, 0.f);
        this->L = glm::vec3(0.f, 0.f, 0.f);
        this->P = glm::vec3(0.f, 0.f, 0.f);
    }

    RigidBody(shared_ptr<BoundingVolume> bv, glm::mat3& r, glm::vec3& v): PhysicState(r, v, bv) {}

    void getVelocityAt(const glm::vec3& p, glm::vec3& normal, glm::vec3& v) {
        bv->normalAt(p, normal);
        v = velocity + glm::cross(angular, p - bv->center);
    }

    glm::vec3 getv() {
        return velocity;
    }

    /* 
        Apply force F on point p on surface of the bv stored,
        update linear & angular mometum
    */
    void updateForce(const glm::vec3& p, const glm::vec3& F) {
        glm::vec3 Fv, Torque;
        float mass = bv->mass;
        this->bv->applyForce(p, F, Fv, Torque);

        //update mometum
        P += DT * Fv; 
        L += 10.f * DT * Torque;
        //update velocity & angular
        velocity = P / mass;
        glm::mat3 Iinv = rotation * bv->IbodyInv() * glm::transpose(rotation);
        angular = Iinv * L;
    }

    void updateForce(const glm::vec3& F) {
        P += DT * F;
        float mass = bv->mass;
        velocity = P / mass;
    }

    void update(float dt) {
        updateRotation(dt);
        updateTransition(dt);
        this->bv->update(rotation, transition);
        //updateForce(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void setShader(shared_ptr<Shader>& shader) {
        shader->setMat3("rotation", rotation);
        shader->setVec3("transition", transition);
        // std::cout<<glm::determinant(rotation)<<std::endl;
        /*std::cout<<rotation[0][0]<<" "<<rotation[0][1]<<" "<<rotation[0][2]<<"\n"
                 <<rotation[1][0]<<" "<<rotation[1][1]<<" "<<rotation[1][2]<<"\n"
                 <<rotation[2][0]<<" "<<rotation[2][1]<<" "<<rotation[2][2]<<"\n"
                 <<std::endl;*/   
    }
};

class PhysicWorld {
public:
    //TODO Use BVH
    std::vector<shared_ptr<PhysicState>> phyobjs;

    void push_back(shared_ptr<PhysicState> phy) {
        phyobjs.push_back(phy);
    }

    void updateForceField() {
        glm::vec3 G(0.f, -1.f, 0.f);
        for(auto const & phy : phyobjs) {
            phy->updateForce(G);
        }
    }

    // Calculation collision, update data in each phystate
    void collisionDetection() {
        for(auto const& phy1 : phyobjs) {
            for(auto const& phy2 : phyobjs) {
                if (phy1 == phy2 || !Collision::intersect(phy1->bv, phy2->bv)) continue;
                // collision happen
                // std::cout<< "collision happen!!!!!"<<std::endl;
                // TODO go back some time to when collision happen
                phy1->update(-1.f / 60.f);
                phy2->update(-1.f / 60.f);

                // TODO calculate intersection point

                glm::vec3 p1, p2;
                Collision::intersectPoint(phy1->bv, phy2->bv, p1, p2);
                // calculate force
                float m1 = phy1->bv->mass;
                float m2 = phy2->bv->mass;
                glm::vec3 n1, v1;
                glm::vec3 n2, v2;
                phy1->getVelocityAt(p1, n1, v1);
                phy2->getVelocityAt(p2, n2, v2);
                
                if(glm::length(v1 - v2) < 0.001f) continue;

                glm::vec3 Vrel1 = glm::dot(v1 - v2, n1) * n1;
                glm::vec3 Vrel2 = glm::dot(v2 - v1, n2) * n2;
                // std::cout<<glm::to_string(dir1)<<" "<<glm::to_string(dir2)<<std::endl;

                glm::vec3 F1 = (-100.f / (1.f/m1 + 1.f/m2) ) * Vrel1;
                glm::vec3 F2 = (-100.f / (1.f/m1 + 1.f/m2) ) * Vrel2;

            
                glm::vec3 dir = glm::normalize(v1 - v2);
                //F1 +=  glm::length(F1) * dir;
                //F2 +=  glm::length(F2) * dir;

                phy1->updateForce(p1, F1);
                phy2->updateForce(p2, F2);
                phy1->update(1.f / 60.f);
                phy2->update(1.f / 60.f);
            }
        }
    }
};
extern PhysicWorld GamePhysic;
#endif