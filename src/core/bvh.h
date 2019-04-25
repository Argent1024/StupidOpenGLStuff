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
    friend int intersectOOBB(const BoundVolume& a, const BoundVolume& b);

};

int intersectOOBB(const BoundVolume& a, const BoundVolume& b) {
    float ra, rb;
    mat3 R, AbsR;

    //Compute rotation matrix expressing b in a's coordinate frame
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            R[i][j] = dot(a.u[i], b.u[j]);
        }
    }
    
    vec3 t = b.c - a.c;
    t = vec3(dot(t, a.u[0]), dot(t, a.u[1]), dot(t, a.u[2]));
    //TODO
    return 0;
}


#endif