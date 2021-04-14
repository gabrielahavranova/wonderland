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
};

uniform Light light;
uniform Material material;

void main () {
	//vec3 light_pos = vec3(2.0f, 20.0f, 1.0f);

	//ambient
	float ambient_strength = 0.1f;
    //vec3 ambient = material.ambient * ambient_strength * light_color;
    vec3 ambient = material.ambient * light.ambient;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(light.position - frag_pos);
	float diff = max(dot(norm, light_direction), 0.0);
	//vec3 diffuse = light_color * (diff * material.diffuse);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// specular
	float specular_strength = 0.5;
	vec3 view_direction = normalize(view_pos - frag_pos);
	vec3 reflect_direction = reflect(-light_direction, norm);

	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	//vec3 specular = specular_strength * spec * light_color * material.specular;
	vec3 specular = light.specular * spec * material.specular;
	
	//FragColor = mix(texture(texture0, tex_coord), texture(texture1, tex_coord), 0.2);
	//FragColor = vec4(1.0f, 6.0f, 1.0f, 0.8f);
	FragColor = texture2D(texture0, tex_coord) * vec4((ambient + diffuse + specular), 1.0f) ;
	
	
	
	

}