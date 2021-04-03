#pragma once
#include "stb_image.h"
#include "Shader.h"
#include "VerticesLib.h"
#include <vector>
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
//#include "glm/glm/glm.hpp"
//#include "glm/glm/gtc/matrix_transform.hpp"
#include "OBJtest.h"

class Object : public OBJtest { 
public: 
	Object(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& name) : OBJtest(vertices, vertices_cnt, indices, indices_cnt, shader, name) {}
		void DrawBoxes () override {
			float s = (float)getTimeSeed();
			glm::mat4 model1 = glm::mat4(1.0f);
			model1 = glm::translate(model1, glm::vec3(1.0f, 1.0f, 1.0f));
			float angle = 20.0f;
			model1 = glm::rotate(model1, s * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			shader->setMat4("model", model1);
			// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
			glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			shader->setFloat("color1", 0.9f);
			shader->setFloat("color2", 0.9f);
			shader->setFloat("color3", 0.0f);
			glDrawElements(GL_TRIANGLES, kukuTriCNT, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
};

class Object2 : public OBJtest {
public:

	Object2(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& name) : OBJtest(vertices, vertices_cnt, indices, indices_cnt, shader, name) {}

	void DrawBoxes() override {

		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(3.0f, -5.0f, 5.0f));
		float angle = -90.0f;
		//if (i < 5) {
		model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

		shader->setMat4("model", model1);
		// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader->setFloat("color1", 0.05f);
		shader->setFloat("color2", 0.2f);
		shader->setFloat("color3", 0.05f);
		glDrawElements(GL_TRIANGLES, planeNTriangles, GL_UNSIGNED_INT, 0);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->setFloat("color1", 0.0f);
		shader->setFloat("color2", 0.0f);
		shader->setFloat("color3", 0.0f);
		glDrawElements(GL_TRIANGLES, planeNTriangles, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

class Object3 : public OBJtest {
public:
	Object3(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& name) : OBJtest(vertices, vertices_cnt, indices, indices_cnt, shader, name) {}

	void DrawBoxes() override {
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
			glm::mat4 model1 = glm::mat4(1.0f);
			//										 vvvvv plane      |  height| distance from camera
			model1 = glm::translate(model1, glm::vec3(0.0f + xses[i] * 4.5f, -5.0f + (i % 4) * 1.5f, 10.0f + xses[49 - i] * 8.0f));
			float angle = -90.0f;
			//if (i < 5) {

			model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			model1 = glm::scale(model1, glm::vec3(5.0f, 5.0f, 7.0f));

			shader->setMat4("model", model1);
			// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
			glBindVertexArray(VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			shader->setFloat("color1", 1.0f);
			shader->setFloat("color2", 0.768f);
			shader->setFloat("color3", 0.768f);
			glDrawElements(GL_TRIANGLES, cylinderNTriangles, GL_UNSIGNED_INT, 0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			shader->setFloat("color1", 0.0f);
			shader->setFloat("color2", 0.0f);
			shader->setFloat("color3", 0.0f);
			glDrawElements(GL_TRIANGLES, cylinderNTriangles, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}};

class Object4 : public OBJtest {
public:
	Object4(const float* vertices, const int vertices_cnt, const unsigned int* indices, const int indices_cnt,
		std::shared_ptr <Shader> shader, const std::string& name) : OBJtest(vertices, vertices_cnt, indices, indices_cnt, shader, name) {}

	void DrawBoxes() override {
		//std::cout << "drawboxes called " << std::endl;
			//for (unsigned int i = 0; i < 10; i++) {

		glm::mat4 model1 = glm::mat4(1.0f);
		model1 = glm::translate(model1, glm::vec3(35.0f, 30.0f, 18.0f));
		model1 = glm::scale(model1, glm::vec3(0.7f, 0.7f, 0.7f));
		float angle = -90.0f;
		//if (i < 5) {
		model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.3f, 0.0f, 0.0f));
		model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(0.0f, 0.0f, 0.5f));
		//}
		//else model1 = glm::rotate(model1, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		shader->setMat4("model", model1);
		// --------------------------v  = indices !!!! CNT !!!! FUCKING HELL!!!!!! 
		glBindVertexArray(VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		shader->setFloat("color1", 0.0f);
		shader->setFloat("color2", 0.0f);
		shader->setFloat("color3", 0.0f);
		glDrawElements(GL_TRIANGLES, torus_001NTriangles, GL_UNSIGNED_INT, 0);

		double x = (int)(getTimeSeed() * 1000) / 1000.0;
		float color = std::abs(std::sin(x));
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		shader->setFloat("color1", 1.0f - color);
		shader->setFloat("color2", color);
		shader->setFloat("color3", 0.2f + color / 4.0f);
		glDrawElements(GL_TRIANGLES, torus_001NTriangles, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//}
	}
};