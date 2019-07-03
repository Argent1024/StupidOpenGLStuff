#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "gameObject.h"
#include "physic/phyworld.h"

// Class for storing gameobj
class Scene {
private:
	std::vector<shared_ptr<GameObj>> objects;
public:
	void render() {
		for (auto const & obj : objects) {
			obj->render();
		}
	}

	/*	TODO */
	shared_ptr<GameObj> createObj(const string& shaderName, const string& shapeName,
		shared_ptr<PhyShape> physhape,
		glm::mat3 rotation,
		glm::vec3 transation,
		const string& texname,
		const PhysicType type) {
		std::shared_ptr<GameObj> testObj = make_shared<GameObject>(shaderName, shapeName, physhape,
			rotation, transation, texname, type);
		objects.push_back(testObj);
		return testObj;
	}

};

extern Scene GameScene;

#endif // ! SCENE_H
