#ifndef GAME_H
#define GAME_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "loader.h"
#include "bvh.h"
#include "gameObject.h"
#include "shape.h"

class Game {
private:
    GLFWwindow* window;
    vector<GameObj*> objs;

    GameObject* createObj(const string& shaderName, const string& shapeName,
                   shared_ptr<BoundingVolume> bv,
                   glm::mat3 rotation,
                   glm::vec3 transation,
                   const string& texname,
                   const PhysicType type) 
    {
        GameObject* testObj = new GameObject(shaderName, shapeName, bv, 
                                              rotation, transation, texname, type);
        this->objs.push_back(testObj);
        return testObj;
    }

public:
    // initialize all stuff, load everything
    int init();
    
    int load();

    // TODO hard code....
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        GameCamera.updateInput(key, action);
    }

    // Game loop
    int run();
};



#endif