#include "particlesys.h"

glm::vec3 gradWpoly(const glm::vec3& x0, const glm::vec3& xi, float h) {
	glm::vec3 x = x0 - xi;
	glm::vec3 dx = 2 * x;
	float r = x.length();

	if (h < r) {
		float c = 315.f / 64.f / 3.1415926f;
		float t = std::powf(h, 2) - std::powf(r, 2);
		float dr = c * (-6 * r * std::powf(t, 2)) / std::powf(h, 9);
		return dr * dx;
	}
	else {
		return glm::vec3(0.f, 0.f, 0.f);
	}
}

glm::vec3 lapWpoly(const glm::vec3& x0, const glm::vec3& xi, float h) {
	glm::vec3 x = x0 - xi;
	glm::vec3 dx = 2 * x;
	float r = x.length();
	if (h > r) {
		float c = 315.f / 64.f / 3.1415926f;
		float t = std::powf(h, 2) - std::powf(r, 2);
		float r2 = std::powf(r, 2);
		float h9 = std::powf(h, 9);

		float dr = c * (-6 * r * std::powf(t, 2)) / std::powf(h, 9);
		// second order of r
		float dr2 = c * (6 * std::powf(t, 2) / h9 - (24 * r2 * t) / h9);

		return 2 * dr + dr2 * glm::pow2(dx);
	}
	else {
		return glm::vec3(0.f, 0.f, 0.f);
	}
	 
}

/*void ParticleSystem::update() {
	const float h = 0.1;
	const int n = particles.size();
	for (int i = 0; i < n; i++) {
		glm::vec3 visocityF(0.f, 0.f, 0.f);

		for (int j = 0; j < n; j++) {
			const glm::vec3& x0 = particles[i]->transition;
			const glm::vec3& xj = particles[j]->transition;


			glm::vec3 lapW = lapWpoly(x0, xj, h);
			//glm::vec3 gradW = gradWpoly()
			
			visocityF += lapW;
		}

		particles[i]->updateForce(visocityF);
	}
}*/
