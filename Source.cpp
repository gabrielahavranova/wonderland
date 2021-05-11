#include "Wonderland.h"
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "Camera.h"
#include "ConstsAndTypes.h"

/*!	
 *		Main program loop and main drawing loop as well. 
 */
int main(int argc, char** argv) {
	Wonderland::initEnviroment();
	Wonderland::createShaders();
	Wonderland::createObjects();
	std::shared_ptr <Shader> last_shader = nullptr;

	GLFWwindow* w = Wonderland::win;
	while (!glfwWindowShouldClose(w)) {
		float current_frame = glfwGetTime();
		Wonderland::delta_time = current_frame - Wonderland::last_frame;
		Wonderland::last_frame = current_frame;

		Wonderland::processInput();

		if (Wonderland::getClickLocation) {
			glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			for (auto& object : Wonderland::simple_scene_objects) {
				if (last_shader != object->shader) {
					Wonderland::setViewAndProjection(object->shader);
					last_shader = object->shader;
				}
				object->Draw();
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			unsigned char color[4];
			glReadPixels(WIN_WIDTH / 2, WIN_HEIGHT / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
			Wonderland::getClickLocation = false;
			Wonderland::shaders["basic"]->use();
			Wonderland::shaders["basic"]->setBool("click_test.perform", false);
			Wonderland::makeClickAction(color);
		}

		glClearColor(0.05f, 0.05f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		for (auto& object : Wonderland::simple_scene_objects) {
			if (last_shader != object->shader) {
				Wonderland::setViewAndProjection(object->shader);
				last_shader = object->shader;
			}
			object->Draw();
		}

		Wonderland::setSkyboxMatrices();
		Wonderland::skybox->draw();

		Wonderland::drawMultimesh();

		glfwSwapBuffers(w);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}