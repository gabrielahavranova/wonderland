#include "Wonderland.h"
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "../glm/glm/glm.hpp"
#include "../glm/glm/gtc/matrix_transform.hpp"
#include "../glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "ConstsAndTypes.h"
//#include "assimp/BaseImporter.h"

int main(int argc, char** argv) {
	Wonderland::initEnviroment();
	Wonderland::createShaders();
	Wonderland::createObjects();

	GLFWwindow* w = Wonderland::win;
	while (!glfwWindowShouldClose( w ) ) {
		float current_frame = glfwGetTime();
		Wonderland::delta_time = current_frame - Wonderland::last_frame;
		Wonderland::last_frame = current_frame;

		Wonderland::processInput();

		glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto& object : Wonderland::objectsTEST) {
			Wonderland::setViewAndProjection(object->shader);
			object->Draw();
		}

		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}