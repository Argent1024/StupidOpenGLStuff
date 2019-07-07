#ifndef PHYSIC_H
#define PHYSIC_H

#include "assert.h"
#include <cmath>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>  
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "oglutil/shader.h"

enum PhysicType {
    NOPHYSIC,
    RIGIDBODY,
    PRATICLE
};

const float DT = 1 / 30.f;


void reorthogonalization(glm::mat3& m);


class PhyShape {
public:
    const float mass;
    glm::vec3 center;

    PhyShape(float mass, glm::vec3 c):mass(mass), center(c) {}

    // Make sure point p is on the shape
    virtual void AssertPoint(const glm::vec3& p) = 0;
    virtual glm::mat3 getIbody() = 0;
    virtual glm::mat3 getIbodyInv() = 0;

    // Apply F at p, return F applied on center Fv and Torque
    virtual void applyForce(const glm::vec3& p, const glm::vec3& F, 
					        glm::vec3& Fv, glm::vec3& Torque) {
        AssertPoint(p);
        glm::vec3 dirv = glm::normalize(p - center);
		Fv = glm::dot(dirv, F) * dirv;
		Torque = glm::cross(p - center, F);
    }

    virtual glm::vec3 normalAt(glm::vec3& p) {
        return p - center;
    }

    virtual void update(glm::mat3 rotation, glm::vec3 transition) {
        center = transition;
    }

	void update(glm::vec3 transition) {
		center = transition;
	}

};

//TODO move rotation outside this class
class PhysicState {
public:
	// The transform
    std::shared_ptr<PhyShape> physhape;
    glm::vec3 transition;
	glm::mat3 rotation;

    PhysicState(std::shared_ptr<PhyShape> physhape) : physhape(physhape) {
		rotation = glm::mat3(1.f);
        transition = glm::vec3(0.f);
    }

    PhysicState(glm::mat3& r, glm::vec3& v, std::shared_ptr<PhyShape> physhape)
        :rotation(r),transition(v),physhape(physhape) {}

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

    virtual void getVelocityAt(const glm::vec3& p, glm::vec3& v) const = 0;
    virtual void update(float dt) = 0;
    virtual void setShader(std::shared_ptr<Shader>& shader) = 0;
    virtual glm::vec3 getv() = 0;
};

class NoPhysic : public PhysicState {
public:
    NoPhysic(std::shared_ptr<PhyShape> physhape): PhysicState(physhape) {}

    NoPhysic(std::shared_ptr<PhyShape> physhape, glm::mat3& r, glm::vec3& v):
        PhysicState(r, v, physhape) {}

    glm::vec3 getv() {
        return glm::vec3(0.f);
    }

    void getVelocityAt(const glm::vec3& p, glm::vec3& v) const {
        //physhape->normalAt(p, normal);
        v = glm::vec3(0.f);
    }

    void update(float dt) {
        return;
    }
    void setShader(std::shared_ptr<Shader>& shader){
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
        //std::cout<<glm::determinant(rotation)<<std::endl;
        reorthogonalization(rotation);
    }

    void updateTransition(float dt) {
        if(glm::length(velocity) <= 0.001f) {
            return;
        }
        transition += dt * velocity;
    }

public:
    RigidBody(std::shared_ptr<PhyShape> physhape): PhysicState(physhape) {
        this->angular = glm::vec3(0.0f, 0.0f, 0.0f);
        this->velocity = glm::vec3(0.f, 0.f, 0.f);
        this->L = glm::vec3(0.f, 0.f, 0.f);
        this->P = glm::vec3(0.f, 0.f, 0.f);
    }

    RigidBody(std::shared_ptr<PhyShape> physhape, glm::mat3& r, glm::vec3& v): PhysicState(r, v, physhape) {}

    void getVelocityAt(const glm::vec3& p, glm::vec3& v) const {
        v = velocity + glm::cross(angular, p - physhape->center);
    }

    glm::vec3 getv() {
        return velocity;
    }

    /* 
        Apply force F on point p on surface of the physhape stored,
        update linear & angular mometum
    */
    void updateForce(const glm::vec3& p, const glm::vec3& F) {
        glm::vec3 Fv, Torque;
        float mass = physhape->mass;
        this->physhape->applyForce(p, F, Fv, Torque);

        //update mometum
        P += DT * Fv; 
        L -= DT * Torque;
        //update velocity & angular
        velocity = P / mass;
        glm::mat3 Iinv = rotation * physhape->getIbodyInv() * glm::transpose(rotation);
        angular = Iinv * L;
    }

    void updateForce(const glm::vec3& F) {
        P += DT * F;
        float mass = physhape->mass;
        velocity = P / mass;
    }

    void update(float dt) {
        updateRotation(dt);
        updateTransition(dt);
        this->physhape->update(rotation, transition);
        //updateForce(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    void setShader(std::shared_ptr<Shader>& shader) {
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