#pragma once
#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
//#include "../glm/glm/glm.hpp"
//#include "../glm/glm/gtc/matrix_transform.hpp"

#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "ObjectBase.h"

const glm::vec3 ones3f = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 zeroes3f = glm::vec3(0.0f, 0.0f, 0.0f);

class YellowBox : public ObjectBase { 
public: 
	YellowBox(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {}
		void DrawObject () override {
			float s = (float)getTimeSeed();
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(1.0f, 1.0f, 1.0f));
			float angle = 20.0f;
			model = glm::rotate(model, s * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
			setModelMatrices(model);
			setMeshMaterial(glm::vec3(0.9f, 0.9f, 0.0f), 0.5f);
			// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
			for (const auto & mesh: meshes) {
				mesh.Draw();
			}
		}
};

class Plane : public ObjectBase {
public:

	Plane(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {
			for (auto & mesh : meshes) {
				mesh.createTexture("grass.png", false);
				//mesh.createTexture("awesomeface.png", false);
			}
	}

	void DrawObject() override {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, -5.0f, 5.0f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		setModelMatrices(model);
		setMeshMaterial(ones3f, 0.0f);
		// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		for (const auto & mesh: meshes) mesh.Draw();
	}
};

class Mushrooms : public ObjectBase {
public:
	Mushrooms(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {}

	void DrawObject() override {
		//std::cout << "drawboxes called " << std::endl;
		const float xses[] = {
			-4, 0, -6, -9, 7, -3, 5, -5, 1, -7,
			 -5, -9, -4, -7, 1, 2, 7, -2, -8, 3,
			 1, -1, 10, -7, -4, 5, -8, -6, 0, 3,
			 -6, -8, -3, -6, 8, -4, 3, 9, -7, -5,
			 5, 7, 8, -8, 7, -6, -5, 9, -3, -2 };

		for (unsigned int i = 0; i < 50; i++) {
			int x = (int)getTimeSeed() % 10;
			int y = (int)getTimeSeed() % 10;
			glm::mat4 model = glm::mat4(1.0f);
			//										 vvvvv plane      |  height| distance from camera
			model = glm::translate(model, glm::vec3(0.0f + xses[i] * 4.5f, -12.0f + (i % 4) * 1.5f, 10.0f + xses[49 - i] * 8.0f));
			float angle = -90.0f;

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(15.0f, 15.0f, 18.0f));

			setModelMatrices(model);
			// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//setMeshMaterial(glm::vec3(1.0f - (float)i * 0.015, 0.768f, 0.768f), 1.0f);
			setMeshMaterial(glm::vec3(1.0f - (float)i * 0.015, 0.768f, 0.768f), ones3f, 0.5f);

			for (const auto& mesh : meshes) {
				mesh.Draw();
			}
		}
	}};

class God : public ObjectBase {
public:
	God(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, shader) {}

	void DrawObject() override {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.0f, 30.0f, 18.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		float angle = -90.0f;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.3f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.5f));

		setModelMatrices(model);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		setMeshMaterial(zeroes3f, 0.0f);
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}
		double x = (int)(getTimeSeed() * 1000) / 1000.0;
		float color = std::abs(std::sin(x));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		setMeshMaterial(glm::vec3(1.0f - color, color, 0.2f + color / 4.0f), 0.3f);
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}
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


class LightSource : public ObjectBase {
public:
	LightSource(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> own_shader, std::shared_ptr <Shader> object_shader) : ObjectBase(vertices, vertices_cnt, indices, indices_cnt, own_shader) {
		this->position = glm::vec3(13.5f, -14.7f, -4.6f);
		object_shader->use();
		object_shader->setVec3("pointlight.ambient",  glm::vec3(0.1f, 0.1f, 0.1f));
		object_shader->setVec3("pointlight.diffuse",  glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
		object_shader->setVec3("pointlight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		object_shader->setVec3("pointlight.position", this->position);

		// constants used for point light attenuation
		object_shader->setFloat("pointlight.constant",  1.0f);
		object_shader->setFloat("pointlight.linear",    0.014);
		object_shader->setFloat("pointlight.quadratic", 0.0019f);

		object_shader->setVec3("dir_light.direction", glm::vec3(2.0f, 100.0f, 1.0f));
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
			mesh.Draw();
		}
		model = glm::translate(model, glm::vec3(5.0f, 20.0f, -4.0f));
		shader->setMat4("model", model);
		shader->setVec3("col", glm::vec3(0.7f, 0.3f, 0.0f));
		for (const auto& mesh : meshes) {
			mesh.Draw();
		}

	}
private: 
	glm::vec3 position;
};