#include "renderManager.h"
#include <string>

using namespace std;
void entityRenderManager::render() {
	// Setup shader program uniforms
	prog.use();
	projectionUniform = getProjection();
	viewUniform = getView();
	diffuseMapUniform = 0;
	specularMapUniform = 1;

	// Iterate trough every entity
	for (iterator it = begin(); it < end(); ++it) {
		modelUniform = (**it).getTranslation();
		
		const type_info &type = typeid(**it);
		if (type == typeid(modelEntity)) {
			modelEntity &entity = *((modelEntity*) *it);
			boneUniform.setArray(entity.boneMatrices, entity.model.getBoneCount());
			entity.model.render();
		} else {
			throw (string("Unknown entity type for renderer: ") + type.name());
		}

	}
}