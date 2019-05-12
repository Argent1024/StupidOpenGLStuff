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

    // Game loop
    int run();
};



#endif