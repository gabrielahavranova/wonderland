#version 330 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D texture0;

uniform vec3 col;

void main () {
	FragColor = vec4(col, 1.0f) ;
}