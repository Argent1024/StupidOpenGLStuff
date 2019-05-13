#ifndef PHYSIC_H
#define PHYSIC_H

#include <iostream>

#include <glm/glm.hpp>
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

    virtual void update() = 0;
    virtual void setShader(shared_ptr<Shader>& shader) = 0;
};

class NoPhysic : public PhysicState {
public:
    NoPhysic(shared_ptr<BoundingVolume> bv): PhysicState(bv) {}

    void update() {
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

    void updateRotation() {
        float x = angular.x;
        float y = angular.y;
        float z = angular.z;
        glm::mat3 crossw(0.f, -z , y,
                         z, 0.f, -x,
                         -y, x, 0.f);
        // Derivation of rotation
        glm::mat3 dR =  crossw * rotation;
        rotation += DT * dR;

        //TODO avoid numerical error
        rotation[0] = glm::normalize(rotation[0]);
        rotation[1] = glm::normalize(rotation[1]);
        rotation[2] = glm::normalize(rotation[2]);
    }

    void updateTransition() {
        transition += DT * velocity;
    }

public:
    RigidBody(shared_ptr<BoundingVolume> bv): PhysicState(bv) {
        this->angular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->velocity = glm::vec3(0.f, 0.f, 0.f);
        this->L = glm::vec3(0.f, 0.f, 0.f);
        this->P = glm::vec3(0.f, 0.f, 0.f);
    }

    RigidBody(shared_ptr<BoundingVolume> bv, glm::mat3& r, glm::vec3& v): PhysicState(r, v, bv) {}

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
        L += DT * Torque;

        //update velocity & angular
        velocity = P / mass;
        glm::mat3 Iinv = rotation * bv->IbodyInv() * glm::transpose(rotation);
        angular = Iinv * L;
    }

    void update() {
        updateRotation();
        updateTransition();
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

#endif