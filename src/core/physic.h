#ifndef PHYSIC_H
#define PHYSIC_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "oglutil/shader.h"

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
    glm::vec3 transition;

    PhysicState() {
        transition = glm::vec3(0.f);
    }

    PhysicState(glm::vec3& v):transition(v) {}

    virtual void update() = 0;
    virtual void setShader(shared_ptr<Shader>& shader) = 0;
};

class NoPhysic : public PhysicState {
public:
    void update() {
        return;
    }
    void setShader(shared_ptr<Shader>& shader){
        return;
    }
};

class RigidBody : public PhysicState {
private:
    // const float mass;
    // const glm::vec3 center;
    glm::mat3 rotation;
    glm::vec3 velocity;
    glm::vec3 anagular;

    void updateAnagularV() {
        float x = anagular.x;
        float y = anagular.y;
        float z = anagular.z;
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

    void updateLinearV() {
        transition += DT * velocity;
    }

public:
    RigidBody(): PhysicState() {
        this->rotation = glm::mat3(1.f);
        this->anagular = glm::vec3(0.1f, 0.1f, 0.1f);
    }

    RigidBody(glm::mat3& r, glm::vec3& v): PhysicState(r, v) {}

    void update() {
        updateAnagularV();
        updateLinearV();
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


class Praticle : public PhysicState {
private:
    glm::vec3 velocity;

    void updateLinearV() {
        transition += DT * velocity;
    }

public:
    Praticle(glm::vec3& t):PhysicState(t){}

   void update() {
       return;
   }

   void setShader(shared_ptr<Shader>& shader) {
       return;
   }

};

#endif