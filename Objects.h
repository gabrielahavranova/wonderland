#pragma once
#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "ConstsAndTypes.h"
//#include "../glm/glm/glm.hpp"
//#include "../glm/glm/gtc/matrix_transform.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "ObjectBase.h"

const glm::vec3 ones3f = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 zeroes3f = glm::vec3(0.0f, 0.0f, 0.0f);

class MushroomBase : public ObjectBase {
	public:
	MushroomBase(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
				 std::shared_ptr <Shader> shader, const std::string & tex_name, std::vector <glm::vec3> &colliders) 
				: ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {
			for (auto& mesh : meshes) mesh.createTexture(tex_name.c_str(), true, true);
	}

	void DrawObject() override {
		for (int i = 0; i < mushroom_cnt; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, this->positions[i]);
			model = glm::rotate(model, glm::radians(-90.0f), x_axis);
			model = glm::rotate(model, glm::radians(angles[i]), rotations[i]);
			model = glm::scale(model, scales[i]);

			setModelMatrices(model);
			setMeshMaterial(ones3f, 0.0f);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			for (const auto& mesh : meshes) mesh.Draw();
		}
	}

	int mushroom_cnt;
	std::vector <glm::vec3> positions; 
	std::vector <glm::vec3> scales; 
	std::vector <glm::vec3> rotations; 
	std::vector <float> angles;
};

class Mush : public MushroomBase { 
public: 
	Mush(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& tex_name, std::vector <glm::vec3>& colliders)
		: MushroomBase(vertices, vertices_cnt, indices, indices_cnt, shader, tex_name, colliders) {

		this->mushroom_cnt = 1;
		this->positions = { glm::vec3(1.0f, 1.0f, 1.0f) };
		this->scales = { glm::vec3(3.0f) };
		this->angles = { 10.0f };
		this->rotations = { z_axis };
		float diameter = 4.0f;
		colliders.emplace_back( 1.0f, 1.0f, diameter);
	}
};

class BlueShrooms : public MushroomBase {
public:
	BlueShrooms(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& tex_name, std::vector <glm::vec3>& colliders)
		: MushroomBase(vertices, vertices_cnt, indices, indices_cnt, shader, tex_name, colliders) {

		this->mushroom_cnt = 4;
		this->positions = { glm::vec3(96.768f, 35.0f, 33.276f), glm::vec3(-2.11738f, 45.0f, 80.3774f),
									 glm::vec3(-63.5439f, 35.0f, 41.4656f), glm::vec3(31.0f, 20.0f, -15.0f) };
		this->scales = { glm::vec3(4.0f), glm::vec3(5.5f), glm::vec3(5.5f), glm::vec3(2.0f) };
		this->angles = { 10.0f, 125.0f, 25.0f, 17.0f };
		this->rotations = { x_axis, z_axis, z_axis, x_axis + z_axis * 0.5f };
		float diameter = 4.0f;
		colliders.emplace_back(29.4098f, -22.5141f, diameter); 
		colliders.emplace_back(95.4057f,  19.5499f, diameter);
		colliders.emplace_back(-11.4088f, 86.2395f, diameter+1.5f);
		colliders.emplace_back(-66.6502f, 33.1402f, diameter+5.5f);
	}
};

class Mushrooms : public MushroomBase {
public:
	Mushrooms(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& tex_name, std::vector <glm::vec3>& colliders)
		: MushroomBase(vertices, vertices_cnt, indices, indices_cnt, shader, tex_name, colliders) {

		this->mushroom_cnt = 6;
		this->positions = { glm::vec3(-94.3434f, 50.001f, -10.4867f), glm::vec3(-71.5002f, 35.001f, 53.5723f),
							glm::vec3(-36.0634f, 5.01f, 65.7154f),    glm::vec3(17.6295f,  17.0f, 76.5066f),
							glm::vec3(58.6476f,  10.001f, 90.6306f),  glm::vec3(59.3222f, 50.001f, 3.98706f) };
		this->scales = { glm::vec3(4.0f), glm::vec3(2.2f), glm::vec3(0.5f), glm::vec3(2.0f),  glm::vec3(1.2f),  glm::vec3(4.0f) };
		this->angles = { 10.0f, 15.0f, 25.0f, 17.0f, 180.0f, 160.0f };
		this->rotations = { x_axis, y_axis + z_axis, z_axis, x_axis + z_axis * 0.5f,  z_axis,  z_axis, };
		const float diameters[6] = { 12.0f, 10.0f, 6.0f, 10.0f, 8.0f, 13.0f };
		for (int i = 0; i < this->mushroom_cnt; i++) colliders.emplace_back(positions[i].x, positions[i].z, diameters[i]);
	}
};

class God : public ObjectBase {
public:
	God(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {
		for (auto& mesh : meshes) {
			mesh.createTexture("monke.png", true, true);
			//mesh.createTexture("awesomeface.png", false);
		}
	}
	void DrawObject() override {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 65.0f, -250.0f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, 1.0f, 0.2f));
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.5f));
		model = glm::scale(model, glm::vec3(50.0f));

		setModelMatrices(model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setMeshMaterial(ones3f, ones3f , 1.0f);
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}
		//double x = (int)(getTimeSeed() * 1000) / 1000.0;
		//float color = std::abs(std::sin(x));
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//	setMeshMaterial(glm::vec3(1.0f - color, color, 0.2f + color / 4.0f), 0.3f);
	//	for (const auto& mesh : meshes) {
	//		mesh.Draw();
	//	}
	}
};

class Lava : public ObjectBase {
public:
	Lava(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, char color_id) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader, color_id) {
			for (auto& mesh : meshes) mesh.createTexture(".\\objects\\lava_diffuse.jpg", false);
			speed = 2.0f;
		//mesh.createTexture("awesomeface.png", false);
	}

	void applyClick() {
		lava_sped_up = !lava_sped_up;
		speed = ((int)(getTimeSeed() * 1000) % 100 / 20.0f) + 1.5f;
		std::cout << "clicked on lava!!! !!! sped: " << speed << std::endl;
	}

	void DrawObject() override {
		shader->setBool("is_lava", true);
		shader->setFloat("lava_sped", speed);
		shader->setVec3("click_test.object_color", glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, -5.5f, -39.0f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.5, 5.5, 5.5));
		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
		for (const auto& mesh : meshes) mesh.Draw();
		shader->setBool("is_lava", false);
		shader->setVec3("click_test.object_color", glm::vec3(0.0f, 0.0f, 0.0f));
	}
private:
	bool lava_sped_up = false;
	float speed;
};

class Flame : public ObjectBase {
public:
	Flame(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {
		for (auto& mesh : meshes) mesh.createTexture(".\\objects\\flame_spritesheet.png", true, true);
		//mesh.createTexture("awesomeface.png", false);
	}

	void DrawObject() override {
		shader->setBool("is_flame", true);
		shader->setBool("is_flame_frag", true);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-33.3198f, -5.0f, -60.7827f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		//float angle = -90.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (const auto& mesh : meshes) mesh.Draw();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-43.3198f, -4.0f, -56.7827f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		for (const auto& mesh : meshes) mesh.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.3198f, -4.3f, -46.7827f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		for (const auto& mesh : meshes) mesh.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-38.3198f, -4.7f, -50.7827f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		for (const auto& mesh : meshes) mesh.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-47.3198f, -4.1f, -53.7827f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		for (const auto& mesh : meshes) mesh.Draw();

		shader->setBool("is_flame", false);
		shader->setBool("is_flame_frag", false);


	}
};


class LightBlueBox : public ObjectBase {
public:
	LightBlueBox(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {}

	void DrawObject() override {
		float s = (float)getTimeSeed();
		
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 1.0f, 1.0f));
		float angle = 30.0f;
		model = glm::rotate(model, s * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		setModelMatrices(model);
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setMeshMaterial(glm::vec3(0.0f, 0.9f, 0.9f), 0.0f);
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}
	}
};


class Stars : public ObjectBase {
public: 
	Stars(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {
	}

	glm::vec3 position = glm::vec3(7.2748f, 64.168f, 17.4503f);
	//const glm::vec3 rot_center = glm::vec3(25.0f, 30.0f, 15.0f);
	const int radius = 5; 

	void DrawObject() override {
		float s = (float)getTimeSeed();
		float angle = 0.2f;
		shader->setVec3("col", glm::vec3(1.0f, 1.0f, 1.0f));


		for (int i = 0; i < star_count; i += 3) {
			glm::mat4 model = glm::mat4(1.0f);
			int rad = i*4 % 8 + 12;
			bool sign = i % 2 == 0 ? 1 : -1;
			float an = 0.1f * 2 / i * sign;
			model = glm::translate(model, glm::vec3(star_positions[i] + glm::cos(an * s) * rad, star_positions[i+1], star_positions[i+2] + glm::sin(s * an) * rad));
			model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
			setModelMatrices(model);
			for (const auto& mesh : meshes) {
				mesh.Draw();
			}
		}

		glm::mat4 model = glm::mat4(1.0f);
		//float angle = 0.2f;

		//this->position = glm::vec3(this->position.x + s*glm::cos(angle) * radius, this->position.y + s*glm::sin(angle) * radius, this->position.z);

		//std::cout << "x: " << this->position.x + glm::cos(s * angle) * radius << ", y: " << this->position.y +  glm::sin(s * angle) * radius << ", z: " << this->position.z << std::endl;
		model = glm::translate(model, glm::vec3(this->position.x + glm::cos(angle *s) * radius, this->position.y , this->position.z + glm::sin(s * angle) * radius));
		model = glm::rotate(model, s * glm::radians(angle), glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		setModelMatrices(model);

		//setMeshMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f);
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}
	}
private:
	const int star_count = 117 / 3; 
};


class LightSource : public ObjectBase {
public:
	LightSource(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> own_shader, std::shared_ptr <Shader> object_shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, own_shader) {
		this->position = glm::vec3(-33.3198f, 5.0f, -56.7827f);
		object_shader->use();
		object_shader->setVec3("pointlight.ambient",  glm::vec3(0.1f, 0.1f, 0.1f));
		object_shader->setVec3("pointlight.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		object_shader->setVec3("pointlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		object_shader->setVec3("pointlight.position", this->position);

		// constants used for point light attenuation
		object_shader->setFloat("pointlight.constant",  1.0f);
		object_shader->setFloat("pointlight.linear",    0.0014);
		object_shader->setFloat("pointlight.quadratic", 0.000007f);

		// direction is set with moon position
		object_shader->setVec3("dir_light.direction", glm::vec3(140.43f, 100.0f, 60.7283f));
		object_shader->setVec3("dir_light.ambient",   glm::vec3(0.15f, 0.15f, 0.15f));
		object_shader->setVec3("dir_light.diffuse",   glm::vec3(0.15f, 0.15f, 0.15f));
		object_shader->setVec3("dir_light.specular",  glm::vec3(0.3f, 0.3f, 0.3f));

	}

	void DrawObject() override {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->position);
		float angle = 30.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

		shader->setMat4("model", model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader->setVec3("col", glm::vec3(0.7f, 0.9f, 0.0f));
		
		for (const auto& mesh : meshes) {
			//mesh.Draw();
		}
		/*model = glm::translate(model, glm::vec3(5.0f, 20.0f, -4.0f));
		shader->setMat4("model", model);
		shader->setVec3("col", glm::vec3(0.7f, 0.3f, 0.0f));
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}*/

	}
private: 
	glm::vec3 position;
};