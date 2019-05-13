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
    shared_ptr<PhysicState> physic;
    unsigned int texture;

public:
    GameObject(const string& shaderName, const string& shapeName,
               shared_ptr<BoundingVolume> bv,
               const string& textureName="",
               const PhysicType type=NOPHYSIC) : GameObj(shaderName, shapeName)
    {
        this->texture = GameTexManger.get(textureName);
        
        if(type == NOPHYSIC) {
            this->physic = make_shared<NoPhysic>(bv);
        } else if(type == RIGIDBODY) {
            this->physic = make_shared<RigidBody>(bv);
        } else {
            cout<< "Wrong type creating physic obj: "<<type<<endl;
        }
    }

    void update() {
        this->physic->update();
    }
    
    void render() {
        //TODO avoid hard code?
        GameCamera.setShader(this->shader);
        this->physic->setShader(this->shader);
        this->shape->render(this->shader, this->texture);
    }
};

#endif