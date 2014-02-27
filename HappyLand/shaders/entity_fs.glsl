#version 430

layout(binding=0) uniform sampler2D diffuseMap;
layout(binding=1) uniform sampler2D specularMap;
uniform mat4 view;

in vec2 st;
in vec3 pos_eye, norm_eye;
out vec4 fragColor;

// Fixed light
vec3 lightPos = vec3(0, 0, 2);
vec3 La = vec3(1,1,1);
vec3 Ld = vec3(1,1,1);
vec3 Ls = vec3(0.5, 0.2, 0.2);

// Surface properties
vec3 Ka = vec3(.1, .1, .1);
vec3 Ks = vec3(1, 1, 1);

void main() {
	vec3 diffuceColor = texture2D(diffuseMap, st).rgb;
	vec3 specularColor = texture2D(specularMap, st).rgb;

	// Ambient
	vec3 Ia = La * Ka * diffuceColor;

	// Diffuse
	vec3 lightPos_eye = vec3(view * vec4(lightPos, 1.0));
	vec3 dirToLight_eye = normalize(lightPos_eye - pos_eye);
	vec3 Id = Ld * diffuceColor * max(dot(dirToLight_eye, norm_eye), 0.0);

	// Specular
	vec3 reflection_eye = reflect(-dirToLight_eye, norm_eye);
	vec3 surfaceToView_eye = normalize(-pos_eye);
	vec3 Is = Ls * specularColor * pow(max(dot(reflection_eye, surfaceToView_eye), 0.0), 100.0);

	fragColor = vec4(Ia + Id + Is, 1.0);
	//fragColor = vec4(texture2D(diffuseMap, st).rgb, 1.0);
}