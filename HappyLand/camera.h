#pragma once

#define CAMERA_Z_NEAR 0.1f
#define CAMERA_Z_FAR 100.0f

class camera;

#include <math/vec.h>
#include <math/mat4.h>

template<class T>
class cameraRenderManager;
class camera {
public:
	camera();
	~camera();

	void resize(unsigned int width, unsigned int height) {
		// Recreate projection
		projection = math::mat4::perspective(fov, 1.0f * width / height, CAMERA_Z_NEAR, CAMERA_Z_FAR);
	}
	void update() {
		math::vec3 dir(
			cos(verticalRotation) * sin(horizontalRotation),
			sin(verticalRotation),
			cos(verticalRotation) * cos(horizontalRotation)
		);
		view = math::mat4::lookAt(position, position + dir, math::vec3(0, 1, 0));
	}
	
	void setPosition(const math::vec3 &_position) {
		position = _position;
	}
	void setRotation(const float horz, const float vert) {
		horizontalRotation = horz;
		verticalRotation = vert;
	}

protected:
	math::mat4 const &getProjection() const {
		return projection;
	}
	math::mat4 const &getView() const {
		return view;
	}

private:
	math::vec3 position;
	math::mat4 projection, view;
	float horizontalRotation, verticalRotation;
	float fov;

	template<typename T>
	friend class cameraRenderManager;
};

