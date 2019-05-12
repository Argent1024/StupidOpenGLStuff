#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <vector>

#include "loader.h"
#include "physic.h"


using namespace std;

class GameObj {
protected:
    //shared_ptr<glm::mat4> transform;
    unique_ptr<PhysicState> physic;
    shared_ptr<Shader> shader; 
    unsigned int texture;

public:
    GameObj(string shaderName, PhysicType type=NOPHYSIC,
            glm::vec3 transition=glm::vec3(0.f)) {
        this->texture = 0;
        this->shader = GameShaderManger.get(shaderName);
        // this->transform = make_shared<glm::mat4>(1.0f);
        if(type == NOPHYSIC) {
            this->physic = make_unique<NoPhysic>();
        } else if(type == RIGIDBODY) {
            this->physic = make_unique<RigidBody>();
        } else if(type == PRATICLE){
            // this->physic == make_unique<Praticle>(transition);
        } else {
            cout<< "Wrong type creating physic obj: "<<type<<endl;
        }
    }

    virtual void render() = 0;

    inline bool setTexture(string& name) {
        this->texture = GameTexManger.get(name);
		return true;
    }

    inline void update() {
        this->physic->update();
    }

    /*inline bool setTransform(shared_ptr<glm::mat4> newTransform) {
        transform = newTransform;
    }

    inline glm::mat4 getTransform() {
        return *transform;
    }

    inline void updateTransform(const glm::mat4& T) {
        *transform = T * *transform;
    }*/
};

/*
    TODO 
*/
struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcor;
    Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 tc):
        pos(p), normal(n),texcor(tc) {}
};

class TriMesh : public GameObj {
public:
    TriMesh(string& shaderName, vector<Vertex>& vertices, 
            vector<unsigned int>& indices, PhysicType type)
        :GameObj(shaderName, type) 
    {
        this->vertices = vertices;
        this->indices = indices;    
        init();       
    }
    void init();
    void render();

private:
    unsigned int VAO;
    unsigned int VBO, EBO;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
};



//TODO UGLY CODE!!!!
//TODO Think how to implement this....
class PointList: public GameObj {
public:
    int n;
    vector<glm::vec3> vertices;
    vector<glm::vec3> color;
    unsigned int VAO, VBO;

    PointList(string& shaderName):GameObj(shaderName) {}

    int addpoint() {
        n++;
        if(n < vertices.size()) {
            return n;
        } else {
            vertices.push_back(glm::vec3(0.f));
            return n;
        }   
    }

    void init() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    
        glBindVertexArray(VAO);
        
        // pos
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6, (void*)0);
/*
        // color
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6, (void*)3);
  */      
        glBindVertexArray(0);
    }

    void render() {
        glBindVertexArray(VAO);
        shader->use();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glDrawElements(GL_POINTS, vertices.size(), GL_UNSIGNED_INT, 0);
    }
    
    void update() {
        return;
    }
};

extern PointList PointRegister;

class Point : public GameObj{
public:
    Point(string& shaderName, glm::vec3 vertice)
        :GameObj(shaderName, PRATICLE, vertice) {
    }

    void render() {
        this->physic->setShader(shader);
    }
};
#endif