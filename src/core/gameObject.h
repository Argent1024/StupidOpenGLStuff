#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

#include <glm/glm.hpp>


#include "loader.h"
#include "phyworld.h"
#include "shape.h"

using namespace std;

class GameObj {
protected:
    shared_ptr<Shader> shader; 
    shared_ptr<Shape> shape;
public:
    GameObj(const string& shaderName, const string& shapeName) 
    {
        this->shader = GameShaderManger.get(shaderName);
        this->shape = GameShapeManger.get(shapeName);   
    }
    
    virtual void applyForce(const glm::vec3& p, const glm::vec3& F) = 0;
    virtual void render() = 0;
    virtual void update() = 0;
};

class GameObject: public GameObj {
private:
    shared_ptr<PhysicState> physic;
    unsigned int texture;

public:
    GameObject(const string& shaderName, const string& shapeName,
               shared_ptr<PhyShape> physhape,
               const string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        std::cout<<"using texuture "<<this->texture<<std::endl;
        if(type == NOPHYSIC) {
            this->physic = make_shared<NoPhysic>(physhape);
        } else if(type == RIGIDBODY) {
            this->physic = make_shared<RigidBody>(physhape);
        } else {
            throw std::invalid_argument( "wrong physic type" );
        }
        GamePhysic.push_back(this->physic);
    }

    GameObject(const string& shaderName, const string& shapeName,
               shared_ptr<PhyShape> physhape,
               glm::mat3 rotation,
               glm::vec3 transation,
               const string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        std::cout<<"using texuture "<<this->texture<<std::endl;
        if(type == NOPHYSIC) {
            this->physic = make_shared<NoPhysic>(physhape, rotation, transation);
        } else if(type == RIGIDBODY) {
            this->physic = make_shared<RigidBody>(physhape, rotation, transation);
        } else {
            throw std::invalid_argument( "wrong physic type" );
        }
        GamePhysic.push_back(this->physic);
    }

    void applyForce(const glm::vec3& p, const glm::vec3& F) {
        this->physic->updateForce(p, F);
    }

    void update() {
        this->physic->update(1.0 / 30.0);
    }
    
    void render() {
        //TODO avoid hard code?
        GameCamera.setShader(this->shader);
        this->physic->setShader(this->shader);
        this->shape->render(this->shader, this->texture);
    }
};

#endif