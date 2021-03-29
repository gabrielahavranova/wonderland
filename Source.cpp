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
	Wonderland::createObjects();

	GLFWwindow* w = Wonderland::win;
	while (!glfwWindowShouldClose( w ) ) {
		float current_frame = glfwGetTime();
		Wonderland::delta_time = current_frame - Wonderland::last_frame;
		Wonderland::last_frame = current_frame;

		Wonderland::processInput();

		glClearColor(0.5f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto & object : Wonderland::objects) {
			object.DrawPrep();
			object.shader->use();
			
			glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
			object.shader->setMat4("projection", projection);

			// camera/view transformation
			glm::mat4 view = Wonderland::camera.GetViewMatrix();
			object.shader->setMat4("view", view);

			for (unsigned int i = 0; i < 10; i++) {
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, cubePositions[i]);
				float angle = 20.0f * i;
				if (i < 5) {
					model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				}
				else model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

				object.Draw(model, 36);
			}
			//object.Draw((float)glfwGetTime());

		}
		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}