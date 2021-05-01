#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;
//layout (location = 1) in vec3 aNorm;
//layout (location = 2) in vec2 aTexCoord;

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
uniform bool is_flame;
uniform Flame flame;

//uniform vec3 light_pos_unif;


void main () {
	gl_Position  = projection * view * model * vec4(aPos, 1.0);
	normal = normal_model * aNorm;
	if (is_lava) {
		tex_coord = tex_coord * 0.7f;
		tex_coord = vec2(aTexCoord.x + cos(time_seed) * 0.7f, aTexCoord.y + sin(time_seed)*0.7f);
	}
		else if (is_flame) {
		//int ts = int(time_seed);
		//int offset_x = ts % 16;
		//int offset_y = ts / 16; 
		//tex_coord = vec2((offset_x + tex_coord.x) * 0.25, (offset_y + tex_coord.y) * 0.25);
		//tex_coord = vec2((flame.x_offset + tex_coord.x) * 0.25, (flame.y_offset + tex_coord.y)* 0.25);
		tex_coord = vec2((flame.x_offset + aTexCoord.x) * 0.25, (flame.y_offset + aTexCoord.y) * 0.25);
		//tex_coord = aTexCoord;

	}
	else {
		tex_coord = aTexCoord;
	}
	frag_pos = vec3(model * vec4(aPos, 1.0));
	//tex_coord = aTexCoord ;
	//tex_coord = aTexCoord * abs(sin(time_seed));
	//light_pos = vec3(view * vec4(light_pos_unif, 1.0));
}