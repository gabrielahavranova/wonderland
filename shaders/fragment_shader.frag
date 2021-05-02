#version 330 core

out vec4 FragColor;

in vec2 tex_coord;
in vec3 normal;
in vec3 frag_pos;

uniform sampler2D texture0;

uniform vec3 view_pos;
uniform int fog;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
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
	vec3 strength;
	float cut_off;
	float outer_cut_off;
};

struct Picking {
	bool on;
	vec3 position;
	vec3 direction;
	float cut_off;
	float outer_cut_off;
};

struct ClickTest {
	bool perform;
	vec3 object_color;
};

uniform DirLight dir_light;
uniform Flashlight flashlight;
uniform PointLight pointlight;

uniform Picking picking;
uniform ClickTest click_test;

uniform Material material;

vec4 applyFog(vec4 res_color) {
	vec4 fog_color = vec4(0.7);
	float intensity = 0.01f;
	
	float dist = length(view_pos - frag_pos);
	float fog_intensity = clamp(exp(-dist * dist * intensity * intensity), 0.0f, 1.0f);
	res_color = res_color * fog_intensity + (1 - fog_intensity) * fog_color;

	return res_color;
}


vec3 getDirLightComponents(vec3 view_direction) {
	vec3 light_direction = normalize(dir_light.direction);
	vec3 norm = normalize(normal);

	float diff = max(dot(norm, light_direction), 0.0);

	vec3 reflect_dir = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_dir), 0.0), material.shininess);

	vec3 ambient  = dir_light.ambient  * material.ambient;
	vec3 diffuse  =	dir_light.diffuse  * diff * material.diffuse;
	vec3 specular =	dir_light.specular * spec * material.specular;

	return vec3(ambient + diffuse + specular);
}


vec3 getPointLightComponents(vec3 view_direction) {
	// compute attenuation
	float dist = length(pointlight.position - frag_pos);
	float attenuation = 1.0 / (pointlight.constant + pointlight.linear * dist + 
						pointlight.quadratic * dist * dist);
	// ambient
	vec3 ambient = material.ambient * pointlight.ambient;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_direction = normalize(pointlight.position - frag_pos);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = pointlight.diffuse * (diff * material.diffuse);
	
	// specular
	//vec3 view_direction = normalize(view_pos - frag_pos);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
	vec3 specular = pointlight.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return vec3(ambient + diffuse + specular);
}


vec3 getFlashlightComponents() {
	vec3 lightDir = normalize(flashlight.position - frag_pos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-flashlight.direction)); 

	if (theta > flashlight.outer_cut_off)	{
		float epsilon   = flashlight.cut_off - flashlight.outer_cut_off;
		float intensity = clamp((theta - flashlight.outer_cut_off) / epsilon, 0.0, 1.0);  
		return flashlight.strength * intensity;
	} else {
		return vec3(0.0, 0.0, 0.0);
	}
}


vec3 applyPicking() {
	vec3 dir = normalize(picking.position - frag_pos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(dir, normalize(-picking.direction)); 

	if (theta > picking.outer_cut_off)	{
		float epsilon   = picking.cut_off - picking.outer_cut_off;
		float intensity = clamp((theta - picking.outer_cut_off) / epsilon, 0.0, 1.0);  
		return vec3(1.0, 0.3, 0.3) * intensity;
	} else {
		return vec3(0.0, 0.0, 0.0);
	}
}


void main () {
	if (!click_test.perform) {
		vec3 view_direction = normalize(view_pos - frag_pos);

		//FragColor = texture2D(texture0, tex_coord) * 
		vec4 tex_color = texture(texture0, tex_coord);
		if (tex_color.a < 0.1) discard;
		vec3 cursor = {0.0, 0.0, 0.0};
		//if (picking_on && frag_pos 
		if (picking.on) {
			cursor = applyPicking();
		}
		
		if (cursor.x > 0.99) {
			FragColor = vec4(cursor, 1.0);
		} else {
			FragColor = tex_color * 
						//vec4(getFlashlightComponents() + getDirLightComponents(view_direction), 1.0);
						vec4(getPointLightComponents(view_direction) + getFlashlightComponents() + getDirLightComponents(view_direction) + cursor, 1.0);
						//vec4(getPointLightComponents(view_direction) + getFlashlightComponents(), 1.0);
			if (fog == 1) {
				FragColor = applyFog(FragColor);
			}
		}
	} else {
		FragColor = vec4(click_test.object_color, 1.0);
	}
}