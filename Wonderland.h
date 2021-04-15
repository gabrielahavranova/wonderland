#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "ConstsAndTypes.h"
#include <iostream>
#include "Camera.h"
#include "Objects.h"
#include "ObjectBase.h"
#include <map>


namespace Wonderland {
	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;
	float delta_time;
	float last_frame;
	GLFWwindow* win;
	bool flashlight_on = false;
	std::vector <std::shared_ptr<ObjectBase>> scene_objects;
	std::map <std::string, std::shared_ptr<Shader>> shaders;
	std::map <int, bool> key_pressed;

	void toggleFlashlight() {
		flashlight_on = !flashlight_on;
	}

	void createObjects() {
		size_t index = scene_objects.size();

		scene_objects.emplace_back(std::make_shared <YellowBox>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <Plane>(planeVertices, planeNVertices * 8, planeTriangles, planeNTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <Mushrooms>(cylinderVertices, cylinderNVertices * 8, cylinderTriangles, cylinderNTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <God>(torus_001Vertices, torus_001NVertices * 8, torus_001Triangles, torus_001NTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <LightBlueBox>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <LightSource>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["light"], shaders["basic"]));
		
		// uniforms
		unsigned int texture1, texture2;

		


		//objects[index].createTexture("container.jpg", texture1);
		//objects[index].createTexture("awesomeface.png", texture2, true);
	}


	void createShaders() {
		shaders.emplace("basic", std::make_shared<Shader>(".\\shaders\\vertex_shader.vert", ".\\shaders\\fragment_shader.frag"));
		shaders.emplace("light", std::make_shared<Shader>(".\\shaders\\vs_light.vert", ".\\shaders\\fs_light.frag"));
	}


	void setViewAndProjection(std::shared_ptr <Shader> shader) {
		std::shared_ptr <Shader> current_shader = shaders["basic"];
		current_shader->use();
		// setup flashlight
		current_shader->setVec3("flashlight.position", camera.Position);
		//shaders["basic"]->setVec3("light_pos_unif", camera.Position);
		//shaders["basic"]->setVec3("flashlight.position", glm::vec3(0.0f, 0.0f, 0.0f));
		current_shader->setVec3("flashlight.direction", camera.Front);
		float theta = flashlight_on ? 12.5f : 0.0f;
		current_shader->setFloat("flashlight.cut_off", glm::cos(glm::radians(theta)));

		if (current_shader != shader) shader->use();
		shader->setVec3("view_pos", camera.Position);
		glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = Wonderland::camera.GetViewMatrix();
		shader->setMat4("view", view);
		shader->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
	}

	void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		camera.ProcessMouseScroll(yoffset);
	}
	void windowResizeCallback(GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
	}

	int initEnviroment() {
		camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		lastX = 800 / 2.0f;
		lastY = 600 / 2.0f;
		firstMouse = true;
		lastX = 0.0f;
		lastY = 0.0f;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "first win", NULL, NULL);
		if (!win) {
			std::cout << "failed to create GLFW window!" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(win);
		glfwSetFramebufferSizeCallback(win, windowResizeCallback);
		glfwSetCursorPosCallback(win, mouseCallback);
		glfwSetScrollCallback(win, scrollCallback);

		glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//initialize GLAD 
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "Failed to initialize GLAD!" << std::endl;
			return -1;
		}
		glEnable(GL_DEPTH_TEST);

		key_pressed.emplace(GLFW_KEY_F1, false);
		key_pressed.emplace(GLFW_KEY_F2, false);
		key_pressed.emplace(GLFW_KEY_L, false);
	}

	void processInput() {
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(win, true);
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, delta_time);
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, delta_time);
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, delta_time);
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, delta_time);
		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F1] = false;
		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F2] = false;
		if (glfwGetKey(win, GLFW_KEY_L) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_L] = false;
		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS && !key_pressed[GLFW_KEY_F1]) {
			camera.ProcessKeyboard(STATIC1, delta_time);
			key_pressed[GLFW_KEY_F1] = true;
		}
		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS && !key_pressed[GLFW_KEY_F2]) {
			camera.ProcessKeyboard(STATIC2, delta_time);
			key_pressed[GLFW_KEY_F2] = true;
		}
		if (glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS && !key_pressed[GLFW_KEY_L]) {
			toggleFlashlight();
			key_pressed[GLFW_KEY_L] = true;
		}
		
	}
};

