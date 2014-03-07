#include "entity.h"

using namespace math;

entity::entity(const ::game &_game, const math::vec3 &_pos, const math::vec3 &_vel) : game(_game), position(_pos), velocity(_vel) {
}


entity::~entity() {
}


void modelEntity::update(double delta) {
	static double time = 0;
	time += delta;
	//boneStatuses[1].scale = vec3(1, 1, cosf((float)(5 * time+2.5))+1);
	boneStatuses[0].scale = boneStatuses[1].scale = boneStatuses[2].scale = boneStatuses[3].scale = vec3(1, 1, 1);
	//boneStatuses[0].scale = vec3(sinf((float)time) + 1, 1, cosf((float)(5 * time)) + 1);
	//boneStatuses[0].versor = boneStatuses[0].versor * versor(0.001, vec3(0, 1, 0));
	//boneStatuses[3].versor = versor(sinf((float)time*3.0f), vec3(1, 0, 0));
	boneStatuses[2].versor = versor(cosf((float)time), vec3(0, 0, 1));
	boneStatuses[1].versor = versor(cosf((float)time*3.0f), vec3(0, 1, 0));

	updateBoneMatrices();
}

void modelEntity::updateBoneMatrices() {
	bone *bones = model.getBones();
	for (unsigned short i = 0; i < model.getBoneCount(); i++) {
		boneStatus &status = boneStatuses[i];
		mat4 &nodeMat = boneMatrices[i];
		bone &node = bones[i];

		mat4 boneMat = mat4(status.versor.getMatrix()) * mat4::scale(status.scale);
		if (node.parentIndex == (unsigned short)-1) {			// The root
			nodeMat = boneMat;
			continue;
		}

		mat4 &parentMat = boneMatrices[node.parentIndex];
		bone &parentNode = bones[node.parentIndex];

		nodeMat = parentMat * mat4::translation(node.head) * boneMat * mat4::translation(-node.head);
	}
}