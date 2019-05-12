#ifndef BVH_H
#define BVH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

// Oriented Bounding Boxes
class BoundVolume {
private:
    glm::vec3 c;
    glm::vec3 u[3];
    glm::vec3 e;
public:
	int intersectOOBB(const BoundVolume& a, const BoundVolume& b) {
		return 0;
	}

};



#endif