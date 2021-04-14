#version 330 core

out vec4 FragColor;

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture0;
//uniform sampler2D texture1;

uniform vec3 col;
uniform vec3 light_color;


//uniform float color1;
//uniform float color2;
//uniform float color3;

void main () {
	vec3 light_pos = vec3(2.0f, 20.0f, 1.0f);

	//ambient
	float ambient_strength = 0.01f;
    vec3 ambient = ambient_strength * light_color;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light_pos - frag_pos);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light_color;

	
	//FragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2);
	//FragColor = vec4(1.0f, 6.0f, 1.0f, 0.8f);
	FragColor = texture2D(texture0, tex_coord) * vec4(col * min((ambient + diffuse), 1), 1.0f) ;
	
	
	
	

}