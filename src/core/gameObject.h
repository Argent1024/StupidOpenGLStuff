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

class GameObj {
protected:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shape> shape;
public:
    GameObj(const std::string& shaderName, const std::string& shapeName)
    {
        this->shader = GameShaderManger.get(shaderName);
        this->shape = GameShapeManger.get(shapeName);   
    }
    
	GameObj(const std::string& shaderName, const std::shared_ptr<Shape> s) {
		this->shape = s;
		this->shader = GameShaderManger.get(shaderName);
	}

    virtual void applyForce(const glm::vec3& p, const glm::vec3& F) = 0;
    virtual void render() = 0;
};

class GameObject: public GameObj {
private:
	std::shared_ptr<PhysicState> physic;
    unsigned int texture;

public:
    GameObject(const std::string& shaderName, const std::string& shapeName,
				std::shared_ptr<PhyShape> physhape,
               const std::string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        std::cout<<"using texuture "<<this->texture<<std::endl;
        if(type == NOPHYSIC) {
            this->physic = std::make_shared<NoPhysic>(physhape);
        } else if(type == RIGIDBODY) {
            this->physic = std::make_shared<RigidBody>(physhape);
		} else {
            throw std::invalid_argument( "wrong physic type" );
        }
        GamePhysic.push_back(this->physic);
    }

    GameObject(const std::string& shaderName, const std::string& shapeName,
			   std::shared_ptr<PhyShape> physhape,
               glm::mat3 rotation,
               glm::vec3 transation,
               const std::string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        std::cout<<"using texuture "<<this->texture<<std::endl;
        if(type == NOPHYSIC) {
            this->physic = std::make_shared<NoPhysic>(physhape, rotation, transation);
        } else if(type == RIGIDBODY) {
            this->physic = std::make_shared<RigidBody>(physhape, rotation, transation);
        } else {
            throw std::invalid_argument( "wrong physic type" );
        }
        GamePhysic.push_back(this->physic);
    }
    
	void applyForce(const glm::vec3& p, const glm::vec3& F) {
		this->physic->updateForce(p, F);
	}

    void render() {
        //TODO avoid hard code?
        GameCamera.setShader(this->shader);
        this->physic->setShader(this->shader);
        this->shape->render(this->shader, this->texture);
    }
};


class ParticleObject : public GameObj {
private:
	std::shared_ptr<ParticleSystem> particlesys;
	unsigned int texture;
public:
	ParticleObject(const std::string& shaderName, std::shared_ptr<Shape> p_shape, const std::string& textureName = "" )
		:GameObj(shaderName, p_shape) {
		this->texture = GameTexManger.get(textureName);
	}
	
	void applyForce(const glm::vec3& p, const glm::vec3& F) {
		std::cout << "Calling applyForce on particle system" << std::endl;
		return;
	}

	void render() {
		GameCamera.setShader(this->shader);
		this->shape->render(this->shader, this->texture);
	}

};
#endif