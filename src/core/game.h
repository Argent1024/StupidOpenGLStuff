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