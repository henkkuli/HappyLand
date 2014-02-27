#include "entity.h"


entity::entity(const ::game &_game, const math::vec3 &_pos, const math::vec3 &_vel) : game(_game), position(_pos), velocity(_vel) {
}


entity::~entity() {
}



void modelEntity::update(double delta) {
	// Update bonematrices
	boneMatrices[1] = boneMatrices[1] * math::mat4::scale(0.9999f, 1.0f, 1.0f);
}