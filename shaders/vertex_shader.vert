#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
//layout (location = 1) in vec3 aNorm;
//layout (location = 2) in vec2 aTexCoord;

out vec2 tex_coord;
out vec3 frag_pos;
out vec3 normal;
out vec3 light_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_model;

//uniform vec3 light_pos_unif;


void main () {
	gl_Position  = projection * view * model * vec4(aPos, 1.0);
	normal = normal_model * aNorm;
	frag_pos = vec3(model * vec4(aPos, 1.0));
	tex_coord = aTexCoord;
	//light_pos = vec3(view * vec4(light_pos_unif, 1.0));
}