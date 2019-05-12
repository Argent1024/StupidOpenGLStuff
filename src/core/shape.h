#ifndef SHAPE_H
#define SHAPE_H

#include <vector>

#include "loader.h"
#include "physic.h"

using namespace std;

// TODO Mutiple instancing
// Shape class 
class Shape {

public:
	virtual void render(Shader& shader, PhysicState& physic) = 0;
};


#endif // !SHAPE_H
