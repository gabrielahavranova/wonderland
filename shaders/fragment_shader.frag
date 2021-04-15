#version 330 core

out vec4 FragColor;

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture0;
//uniform sampler2D texture1;

//uniform vec3 col;
//uniform vec3 light_color;

uniform vec3 view_pos;

//uniform float color1;
//uniform float color2;
//uniform float color3;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Light light;
uniform Material material;

void main () {
	// point light attribs
	float dist = length(light.position - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + 
						light.quadratic * dist * dist);


	// ambient
    vec3 ambient = material.ambient * light.ambient;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light.position - frag_pos);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	float specular_strength = 0.5;
	vec3 view_direction = normalize(view_pos - frag_pos);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	vec3 specular = light.specular * spec * material.specular;
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	FragColor = texture2D(texture0, tex_coord) * vec4((ambient + diffuse + specular), 1.0f) ;
}