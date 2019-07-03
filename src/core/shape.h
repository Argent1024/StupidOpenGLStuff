#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
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
		
		std::cout << "initlizing TriMesh with " << vertices.size() 
				  << "vertices, "<< indices.size() / 3 << "triangles"<<std::endl;
		/*
		for(auto const& i : indices) {
			std::cout<<i<<" ";
		}
		std::cout<<std::endl;
		*/
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
	const vector<unsigned int> indices;

	void init() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		// indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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

class BallHelper {
public:
	static inline unsigned int sphereCoorToIndex(int tcount, int pcount, int sample) {
		return tcount * sample + pcount;
	}

	static void initVertices(int sample, float r, vector<Vertex>& vertices, vector<unsigned int>& indices) {
		// Get sample * sample vertices
		double twoPi = 2 * 3.1415926;
		double Pi = 3.1415926;
		double n = sample + 1; // avoid corner case ?
		// create vertices
		for(int tcount = 0; tcount < sample; tcount++) {
			for(int pcount = 0; pcount < sample; pcount++) {
				// uniform sample point
				double cosTheta = 2.0 * (double)(tcount + 1) / n - 1.0;
				double theta = glm::acos(cosTheta);
				double sinTheta = glm::sin(theta);
				/*
				double Theta = twoPi * tcount / n - Pi;
				double cosTheta = glm::cos(Theta);
				double sinTheta = glm::sin(Theta);
				*/
				double phi = twoPi * pcount / sample;
				double sinPhi = glm::sin(phi);
				double cosPhi = glm::cos(phi);
				float x = r*sinTheta*cosPhi;
				float y = r*sinTheta*sinPhi;
				float z = r*cosTheta;
				glm::vec3 pos(x, y, z);
				//TODO fix coordinate
				double tex_x = phi / twoPi;
				double tex_y = theta / Pi;
				glm::vec2 texcoor(tex_x, tex_y);
				// normal equals to pos
				vertices.push_back(Vertex(pos, glm::normalize(pos), texcoor));
			}
		}

		unsigned int n_index = vertices.size();
		glm::vec3 northpole(0.f, 0.f, r);
		glm::vec2 tex_northpole(0.5f, 0.f);
		vertices.push_back(Vertex(northpole, glm::normalize(northpole), tex_northpole));

		unsigned int s_index = vertices.size();
		glm::vec3 sourthpole(0.f, 0.f, -r);
		glm::vec2 tex_sourthpole(0.5f, 1.f);
		vertices.push_back(Vertex(sourthpole, glm::normalize(sourthpole), tex_sourthpole));

		// create index
		for(int tcount = 1; tcount < sample; tcount++) {
			for(int pcount = 0; pcount < sample; pcount++) {
				unsigned int v1 = sphereCoorToIndex(tcount - 1, pcount, sample);
				unsigned int v2 = sphereCoorToIndex(tcount - 1, (pcount + 1) % sample, sample);
				unsigned int v3 = sphereCoorToIndex(tcount, pcount, sample);
				unsigned int v4 = sphereCoorToIndex(tcount, (pcount + 1) % sample, sample);
				indices.insert(indices.end(), {v1, v2, v3, 
											   v2, v3, v4});
			}
		}
		for(int pcount = 0; pcount < sample; pcount++) {
			unsigned int n1 = sphereCoorToIndex(sample - 1, pcount, sample);
			unsigned int n2 = sphereCoorToIndex(sample - 1, (pcount + 1) % sample, sample);
			unsigned int s1 = sphereCoorToIndex(0, pcount, sample);
			unsigned int s2 = sphereCoorToIndex(0, (pcount + 1) % sample, sample);
			indices.insert(indices.end(), {n1, n2, n_index, 
										   s1, s2, s_index});
		}
	}
};

class ShapeManager {
public:
	map<string, shared_ptr<Shape>> shapes;
	
	shared_ptr<Shape> get(const string& name) {
		assert(shapes.find(name) != shapes.end());
		return shapes[name];
	}
		
	bool load(const string& name, vector<Vertex>& vertices, vector<unsigned int>& indices) {
		assert(shapes.find(name) == shapes.end());
		shapes[name] = make_shared<TriMesh>(vertices, indices);
		return true;
	}

};
#endif // !SHAPE_H
