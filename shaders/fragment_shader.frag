#version 330 core

out vec4 FragColor;

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;
//in vec3 light_pos;

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


struct Flashlight {
	vec3 position;
	vec3 direction;
	float cut_off;
};

uniform Flashlight flashlight;
uniform Light light;
uniform Material material;

vec3 getPointLightComponents() {
	// compute attenuation
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
	vec3 view_direction = normalize(view_pos - frag_pos);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	vec3 specular = light.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec3(ambient + diffuse + specular);
}


vec3 getFlashlightComponents() {
	vec3 lightDir = normalize(flashlight.position - frag_pos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-flashlight.direction)); 
	if (theta > flashlight.cut_off)	{
		return vec3(0.2, 0.2, 0.2);
	} else {
	return vec3(0.0, 0.0, 0.0);
	}
}



void main () {
	FragColor = texture2D(texture0, tex_coord) * 
				vec4(getPointLightComponents() + getFlashlightComponents(), 1.0);
}