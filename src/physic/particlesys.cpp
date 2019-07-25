#include "particlesys.h"

float Wpoly(float r, float h) {
	if (h < r) {
		return 0.f;
	}
	return 315.f / 64.f / 3.1415926f / std::powf(h, 9) * std::powf(h * h - r * r, 3);
}

float gradWpoly(float r, float h) {
	if (h > r) {
		float c = 315.f / 64.f / 3.1415926f;
		float t = std::powf(h, 2) - std::powf(r, 2);
		float dr = c * (-6 * r * std::powf(t, 2)) / std::powf(h, 9);
		return dr;
	}
	else {
		return 0.f;
	}
}

float gradSpiky(float r, float h) {
	if (h > r) {
		float c = -45.f / 3.1415926f / std::powf(h, 6);
		return c * std::powf(h - r, 2);
	}
	else {
		return 0.f;
	}
}

/*
glm::vec3 lapWpoly(const glm::vec3& x0, const glm::vec3& xi, float h) {
	glm::vec3 x = x0 - xi;
	float r = glm::length(x);
	glm::vec3 dx = x / r;
	// second order of x
	glm::vec3 dx2 = glm::vec3(1.f / r) - 1.f / std::powf(r, 3) * glm::pow2(dx);
	if (h > r) {
		float c = 315.f / 64.f / 3.1415926f;
		float t = std::powf(h, 2) - std::powf(r, 2);
		float r2 = std::powf(r, 2);
		float h9 = std::powf(h, 9);

		float dr = c * (-6 * r * std::powf(t, 2)) / std::powf(h, 9);
		// second order of r
		float dr2 = c * (6 * std::powf(t, 2) / h9 - (24 * r2 * t) / h9);

		//return dr2 * glm::pow2(dx);
		return dr * dx2 + dr2 * glm::pow2(dx);
	}
	else {
		return glm::vec3(0.f, 0.f, 0.f);
	}	 
}
*/

// laplacian for calculate visocity
float laplacianVisco(float r, float h) {
	if (h > r) {
		float c = 45.f / 3.1415926f;
		return c * (h - r) * std::powf(h, 6);
	}
	else {
		return 0.f;
	}

}

void  ParticleSystem::getMax(glm::vec3& m, const glm::vec3& p, int d) {
	m[d] = std::max(m[d], p[d] + r);
}

void ParticleSystem::getMin(glm::vec3& m, const glm::vec3& p, int d) {
	m[d] = std::min(m[d], p[d] - r);
}

PS_Node_PTR ParticleSystem::createRoot() {
	//TODO Use a cleverer method
	glm::vec3 x1(0.f);
	glm::vec3 x2(0.f);
	glm::vec3 y1(0.f);
	glm::vec3 y2(0.f);
	glm::vec3 z1(0.f);
	glm::vec3 z2(0.f);
	for (int i = 0; i < particles.size(); i++) {
		const std::shared_ptr<ParticleState> p = particles[i];
		const glm::vec3& point = p->transition;
		getMax(x1, point, 0);
		getMax(y1, point, 1);
		getMax(z1, point, 2);

		getMin(x2, point, 0);
		getMin(y2, point, 1);	
		getMin(z2, point, 2);
	}

	glm::vec3& c = x1 + x2 + y1 + y2 + z1 + z2;
	glm::vec3 dirx(1.f, 0.f, 0.f);
	glm::vec3 diry(0.f, 1.f, 0.f);
	glm::vec3 dirz(0.f, 0.f, 1.f);
	float lx = x1[0] - x2[0];
	float ly = y1[1] - y2[1];
	float lz = z1[2] - z2[2];
	return std::make_shared<SPNode<int>>(c, dirx, diry, dirz, lx, ly, lz);
}



PS_Node_PTR ParticleSystem::createSPTree(int height) {
	nodeList.clear();
	PS_Node_PTR root = createRoot();
	PS_Node_PTR node = root;

	int h = 1;
	int sum = 0;
	std::cout << "Creating SPTree, nodes has praticle:";
	// create tree
	while (node) {
		if (h == height) {
			// leaf node
			node->isLeaf = true;
			node->visited = true;
			for (int i = 0; i < particles.size(); i++ ) {
				if (insideBV(particles[i], node)) { node->v.push_back(i); }
			}
			nodeList.push_back(node);
			std::cout << node->v.size() <<" ";
			sum += node->v.size();
			node = node->parent;
			h--;
		} else {
			if (node->left && node->right) {
				// if sub node already created
				// determined whether right child has visited
				if (node->left->visited && node->right->visited) {
					node->visited = true;
					node = node->parent;
					h--;
				}
				else {
					node = node->right;
					h++;
				}
			}
			else {
				// create sub node
				createSubNode(node);
				node = node->left;
				h++;

			}
		}
	}
	std::cout << std::endl;
	std::cout << "SPTree finished, with particle: " << sum << std::endl;
	return root;
}

void ParticleSystem::updatePressureDensity() {
	const float h = 1.f;
	const int n = particles.size();
	std::vector<float> pressure;
	
	//TODO DENSITY
	for (int i = 0; i < n; i++) {
		float p = 0;
		float d = 0;
		for (int j = 0; j < n; j++) {
			if (i == j) continue;

			const glm::vec3& xi = particles[i]->transition;
			const glm::vec3& xj = particles[j]->transition;
			float r = glm::length(xi - xj);

			float pj = particles[j]->pressure;

			float w = Wpoly(r, h);
			
			p += w * pj;
		}
		pressure.push_back(p);
	}

	for (int i = 0; i < n; i++) {
		std::cout << pressure[i] << " ";
		particles[i]->pressure = pressure[i];
	}
	std::cout << std::endl;
}


void ParticleSystem::update() {
	createSPTree(8);
	//updatePressureDensity();
	const float h = .5f;
	const int n = particles.size();
	int i, j;
	std::vector<glm::vec3> forces(n, glm::vec3(0.f));

	for (PS_Node_PTR& node_ptr : nodeList) {
		std::vector<int>& pindex = node_ptr->v; // particle index list in that bv

		for (int x = 0; x < pindex.size(); x++) {
			glm::vec3 visocityF(0.f, 0.f, 0.f);
			glm::vec3 pressureF(0.f, 0.f, 0.f);
			glm::vec3 surfaceF(0.f, 0.f, 0.f);
			i = pindex[x];
			for (int y = 0; y < pindex.size(); y++) {
				if (x == y) continue;
				j = pindex[y];

				const glm::vec3& xi = particles[i]->transition;
				const glm::vec3& xj = particles[j]->transition;
				const glm::vec3& vi = particles[i]->velocity;
				const glm::vec3& vj = particles[j]->velocity;
				const float pi = particles[i]->pressure;
				const float pj = particles[j]->pressure;

				float r = glm::length(xi - xj);

				float wPressure = gradSpiky(r, h);
				pressureF += 0.001f * wPressure * pj * (xi - xj) / r;

				float lapV = laplacianVisco(r, h);
				//glm::vec3 gradW = gradWpoly()
				visocityF += 0.01f * lapV * (vj - vi);
			}
			//std::cout << glm::to_string(visocityF) << "," << glm::to_string(pressureF) << std::endl;
			forces[i] = (visocityF - pressureF);
		}
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i]->updateForce(forces[i]);
	}
	std::cout << std::endl;
}
