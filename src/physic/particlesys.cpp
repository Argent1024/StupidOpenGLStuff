#include "particlesys.h"

float Wpoly(float r, float h) {
	if (h < r) {
		return 0;
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

void ParticleSystem::updatePressureDensity() {
	const float h = 0.5f;
	const int n = particles.size();
	std::vector<float> pressure;
	std::vector<float> density;
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
			float dj = particles[j]->density;

			float w = Wpoly(r, h);
			p += w * pj / dj;
		}
		pressure.push_back(p);
	}

	for (int i = 0; i < n; i++) {
		particles[i]->pressure = pressure[i];
	}
}


void ParticleSystem::update() {
	updatePressureDensity();
	const float h = 0.4f;
	const int n = particles.size();
	for (int i = 0; i < n; i++) {
		glm::vec3 visocityF(0.f, 0.f, 0.f);
		glm::vec3 pressureF(0.f, 0.f, 0.f);
		glm::vec3 surfaceF(0.f, 0.f, 0.f);
		for (int j = 0; j < n; j++) {
			if (i == j) continue;

			const glm::vec3& xi = particles[i]->transition;
			const glm::vec3& xj = particles[j]->transition;
			const glm::vec3& vi = particles[i]->velocity;
			const glm::vec3& vj = particles[j]->velocity;

			float r = glm::length(xi - xj);



			float gradW = gradWpoly(r, h);
			pressureF += 0.01f * gradW * (xj - xi) * particles[j]->pressure;

			float lapV = laplacianVisco(r, h);
			//glm::vec3 gradW = gradWpoly()
			visocityF += 0.5f * lapV * (vj - vi);
		}

		particles[i]->updateForce(visocityF - pressureF);
	}
}
