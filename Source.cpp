#include "Wonderland.h"
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "ConstsAndTypes.h"
#include "VerticesLib.h"
#include "Object.h"
#include "assimp/BaseImporter.h"

int main(int argc, char** argv) {
	Wonderland::initEnviroment();

	std::shared_ptr<Shader> shader_ptr = std::make_shared<Shader>(".\\shaders\\vertex_shader.txt", ".\\shaders\\fragment_shader.txt");
	Object obj(cube_vertices, shader_ptr);

	// uniforms
	unsigned int texture1, texture2;
	obj.createTexture("container.jpg", texture1);
	obj.createTexture("awesomeface.png", texture2, true);


	GLFWwindow* w = Wonderland::win;
	while (!glfwWindowShouldClose( w ) ) {
		float current_frame = glfwGetTime();
		Wonderland::delta_time = current_frame - Wonderland::last_frame;
		Wonderland::last_frame = current_frame;

		Wonderland::processInput();

		glClearColor(0.5f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader_ptr->use();

		glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader_ptr->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = Wonderland::camera.GetViewMatrix();
		shader_ptr->setMat4("view", view);

		glBindVertexArray(obj.VAO);

		for (unsigned int i = 0; i < 10; i++) {

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i < 5) {
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			else model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

			glUniformMatrix4fv(glGetUniformLocation(shader_ptr->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &obj.VAO);

	glDeleteBuffers(1, &obj.VBO);
	glDeleteProgram(shader_ptr->ID);


	glfwTerminate();

	return 0;
}