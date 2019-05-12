#ifndef SHAPE_H
#define SHAPE_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "assert.h"

#include "loader.h"
#include "physic.h"

using namespace std;

// TODO Mutiple instancing
// Shape class 
class Shape {

public:
	virtual void render(const shared_ptr<Shader> shader, const int& texture) = 0;
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texcor;
    Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 tc):
        pos(p), normal(n),texcor(tc) {}
};

class TriMesh : public Shape {
public:
	TriMesh(vector<Vertex>& vertices, vector<unsigned int>& indices)
		:vertices(vertices), indices(indices)
	{
		init();
	}

	void render(const shared_ptr<Shader> shader, const int& texture) {
		shader->use();
		if(texture) {
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	unsigned int VAO;
    unsigned int VBO, EBO;
	const vector<Vertex> vertices;
	const vector<unsigned int>& indices;

	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		// set the vertex attribute pointers
		// vertex Positions
		glEnableVertexAttribArray(0);	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// vertex Normals
		glEnableVertexAttribArray(1);	
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		
		// texture cor
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcor));

		// reset stuff
		glBindVertexArray(0);
		return;
	}

};

class ShapeManager {
public:
	map<string, shared_ptr<Shape>> shapes;
	
	shared_ptr<Shape> get(const string& name) {
		assert(shapes.find(name) != shapes.end());
		return shapes[name];
	}
		
	bool load(const string& name, const TriMesh& shape) {
		assert(shapes.find(name) == shapes.end());
		shapes[name] = make_shared<TriMesh>(shape);
	}

};
#endif // !SHAPE_H
