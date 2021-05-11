#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

struct Flame {
	int x_offset;
	int y_offset;
};

out vec2 tex_coord;
out vec3 frag_pos;
out vec3 normal;
out vec3 light_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal_model;

uniform float time_seed;
uniform bool is_lava;
uniform float lava_sped;
uniform bool is_flame;
uniform Flame flame;


void main () {
	gl_Position  = projection * view * model * vec4(aPos, 1.0);
	normal = normal_model * aNorm;
	if (is_lava) {
		tex_coord = tex_coord * 0.7f;
		tex_coord = vec2(aTexCoord.x + cos(lava_sped * time_seed) * 0.7f, aTexCoord.y + sin(lava_sped * time_seed)*0.7f);
	}
		else if (is_flame) {
		
		tex_coord = vec2((flame.x_offset + aTexCoord.x) * 0.25, (flame.y_offset + aTexCoord.y) * 0.25);
	}
	else {
		tex_coord = vec2(aTexCoord.x, aTexCoord.y);
	}
	frag_pos = vec3(model * vec4(aPos, 1.0));
}