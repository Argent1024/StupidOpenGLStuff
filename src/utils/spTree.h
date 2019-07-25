#ifndef SPTREE_H
#define SPTREE_H

#include <vector>
#include <map>
#include <memory>

#include <glm/glm.hpp>  
#include <glm/ext.hpp>

#include "bvh.h"

template <class T>
class SPNode : public BoxBV {
public:
	bool isLeaf;
	bool visited;
	std::vector<T> v;
	std::shared_ptr<SPNode> left;
	std::shared_ptr<SPNode> right;
	std::shared_ptr<SPNode> parent;

	SPNode(glm::vec3 c, glm::vec3 x, glm::vec3 y, glm::vec3 z,
		float lenx, float leny, float lenz)
		: BoxBV(c, x, y, z, lenx, leny, lenz) , isLeaf(false), visited(false) {}

};

// split node into two part by max len dir
template <class T>
void createSubNode(std::shared_ptr<SPNode<T>> node) {
	glm::vec3 dir;
	float lx, ly, lz;
	float lenx = node->lenx;
	float leny = node->leny;
	float lenz = node->lenz;

	float len;
	if (lenx >= leny && lenx >= lenz) {
		dir = node->x;
		len = lenx / 2.f;
		lx = len;
		ly = leny;
		lz = lenz;
	}
	else if (leny >= lenx && leny >= lenz) {
		dir = node->y;
		len = leny / 2.f;
		lx = lenx;
		ly = len;
		lz = lenz;
	}
	else if (lenz >= lenx && lenz >= leny) {
		dir = node->z;
		len = lenz / 2.f;
		lx = lenx;
		ly = leny;
		lz = len;
	}
	else {
		// shouldn't reach here
		std::cout << "NONE OF CONDITION SATISFIED IN CREAATING SUBNODE" << std::endl;
	}

	glm::vec3 c1 = node->center - dir * len;
	glm::vec3 c2 = node->center + dir * len;

	node->left = std::make_shared<SPNode<T>>(c1, node->x, node->y, node->z, lx, ly, lz);
	node->right = std::make_shared<SPNode<T>>(c2, node->x, node->y, node->z, lx, ly, lz);

	node->left->parent = node;
	node->right->parent = node;
}

#endif // !SPTREE_H
