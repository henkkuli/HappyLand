#version 430

in layout (location = 0) vec3 pos;
in layout (location = 1) vec2 uv;
in layout (location = 2) vec3 norm;
in layout (location = 3) vec4 index;
in layout (location = 4) vec3 weight;

uniform float time;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform mat4 bones[4];

out vec2 st;
out vec3 pos_eye, norm_eye;

void main() {
	st = uv;

	mat4 joint = bones[int(index[0])] * weight[0];
	mat4 viewModel = view*model*joint;
	pos_eye = vec3(viewModel * vec4(pos, 1.0));
	norm_eye = mat3(viewModel) * norm;
	gl_Position = proj * vec4(pos_eye, 1.0);
}