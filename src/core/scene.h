#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include "gameObject.h"
#include "physic/phyworld.h"

// Class for storing gameobj
class Scene {
private:
	std::vector<std::shared_ptr<GameObj>> objects;
public:
	void render() {
		for (auto const & obj : objects) {
			obj->render();
		}
	}

	/*	TODO */
	std::shared_ptr<GameObj> createObj(const std::string& shaderName, const std::string& shapeName,
		std::shared_ptr<PhyShape> physhape,
		glm::mat3 rotation,
		glm::vec3 transation,
		const std::string& texname,
		const PhysicType type) {
		std::shared_ptr<GameObj> testObj = std::make_shared<GameObject>(shaderName, shapeName, physhape,
			rotation, transation, texname, type);
		objects.push_back(testObj);
		return testObj;
	}

};

extern Scene GameScene;

#endif // ! SCENE_H
