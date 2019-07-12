#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "oglutil/shader.h"

const float CameraSpeed = 0.1f;

class Camera {
private:
    glm::mat4 view;
    glm::mat4 projection;
    
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    void updateView() {
         view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

public:
    Camera() {
        cameraPos   = glm::vec3(0.0f, 0.0f,  10.0f);
        cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f); 

        updateView();
        projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.0f);
    }
    
    void setShader(std::shared_ptr<Shader>& shader) {
        shader->setMat4("view", view);
        shader->setMat4("projection", projection);
    }

    void updateInput(int key, int action) {
        if (key == GLFW_KEY_W && action == GLFW_PRESS) {
            cameraPos += cameraFront * CameraSpeed; 
        } else if(key == GLFW_KEY_A && action == GLFW_PRESS) {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * CameraSpeed; 
        } else if(key == GLFW_KEY_S && action == GLFW_PRESS) {
            cameraPos -= cameraFront * CameraSpeed; 
        } else if(key == GLFW_KEY_D && action == GLFW_PRESS) {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * CameraSpeed; 
        }  
        updateView();
    }
};

#endif