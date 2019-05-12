#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

#include "loader.h"
#include "physic.h"
#include "shape.h"

using namespace std;

class GameObj {
protected:
   
    // unique_ptr<PhysicState> physic;
    shared_ptr<Shader> shader; 
    shared_ptr<Shape> shape;
public:
    GameObj(const string& shaderName, const string& shapeName) 
    {
        
        this->shader = GameShaderManger.get(shaderName);
        this->shape = GameShapeManger.get(shapeName);   
    }

    virtual void render() = 0;
    virtual void update() = 0;
};

class GameObject: public GameObj {
private:
    unique_ptr<PhysicState> physic;
    unsigned int texture;

public:
    GameObject(const string& shaderName, const string& shapeName,
               const string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        
        if(type == NOPHYSIC) {
            this->physic = make_unique<NoPhysic>();
        } else if(type == RIGIDBODY) {
            this->physic = make_unique<RigidBody>();
        } else {
            cout<< "Wrong type creating physic obj: "<<type<<endl;
        }
    }

    void update() {
        this->physic->update();
    }
    
    void render() {
        this->physic->setShader(this->shader);
        this->shape->render(this->shader, this->texture);
    }
};

#endif