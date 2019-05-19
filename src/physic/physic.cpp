#include "physic.h"
void reorthogonalization(glm::mat3& m){
 glm::vec3& x = m[0];
    glm::vec3& y = m[1];
    glm::vec3& z = m[2];
    float error = 0.5f * glm::dot(x, y);
    glm::vec3 x_ort = x - error * y;
    glm::vec3 y_ort = y - error * x;
    glm::vec3 z_ort = glm::cross(x_ort, y_ort);
    m[0] = glm::normalize(x_ort);
    m[1] = glm::normalize(y_ort);
    m[2] = glm::normalize(z_ort);
}