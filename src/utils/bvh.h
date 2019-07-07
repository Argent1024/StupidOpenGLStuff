#ifndef BVH_H
#define BVH_H

#include <glm/glm.hpp>  
#include <glm/ext.hpp>

class BoxBV {
public:
	glm::vec3 center;
	glm::vec3 x;
	glm::vec3 y;
	glm::vec3 z;
	float lenx;
	float leny;
	float lenz;

	BoxBV(glm::vec3 c, glm::vec3 x, glm::vec3 y, glm::vec3 z,
		float lenx, float leny, float lenz)
		: center(c), x(x), y(y), z(z), lenx(lenx), leny(leny), lenz(lenz) {}

	float volume() {
		return lenx * leny * lenz;
	}
};


#endif // !BVH_H
