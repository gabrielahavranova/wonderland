#version 330 core

out vec4 FragColor;

in vec2 tex_coord;

uniform sampler2D texture0;
//uniform sampler2D texture1;
//uniform float color1;
//uniform float color2;
//uniform float color3;

uniform vec3 col;

void main () {
	//FragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2);
	//FragColor = vec4(1.0f, 6.0f, 1.0f, 0.8f);
	//FragColor = texture2D(texture0, tex_coord) * vec4(col, 1.0f) ;
	FragColor = vec4(col, 1.0f) ;
}