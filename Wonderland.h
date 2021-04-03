#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "ConstsAndTypes.h"
#include <iostream>
#include "Camera.h"
#include "Object.h"
#include "Object2.h"
#include "Object3.h"
#include "Objec4.h"
#include "OBJtest.h"
#include <map>


namespace Wonderland {
	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;
	float delta_time;
	float last_frame;
	GLFWwindow* win;
	//std::vector <Object> objects;
	//std::vector <Object2> objects2;
	//std::vector <Object3> objects3;
	//std::vector <Object4> objects4;
	std::vector <std::shared_ptr<OBJtest>> objectsTEST;
	//std::vector <OBJtest> objectsTEST;
	std::map <std::string, std::shared_ptr<Shader>> shaders;

	void createBoxes() {
		size_t index = objectsTEST.size();
		objectsTEST.emplace_back(std::make_shared <Object>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"], "box"));

		// uniforms
		unsigned int texture1, texture2;
		//
		
		//objects[index].createTexture("container.jpg", texture1);
		//objects[index].createTexture("awesomeface.png", texture2, true);
	}

	void createTorus() {
		size_t index = objectsTEST.size();
		//objects2.emplace_back(torusVert, torusN * 8, torusTri, torusTriCNT, shaders["basic2"], "torus");
		objectsTEST.emplace_back(std::make_shared <Object2>(planeVertices, planeNVertices * 8, planeTriangles, planeNTriangles, shaders["basic2"], "plane"));
		objectsTEST.emplace_back(std::make_shared <Object3>(cylinderVertices, cylinderNVertices* 8, cylinderTriangles, cylinderNTriangles, shaders["basic2"], "plane"));
		objectsTEST.emplace_back(std::make_shared <Object4>(torus_001Vertices	, torus_001NVertices* 8, torus_001Triangles, torus_001NTriangles, shaders["basic2"], "plane"));
		//OBJtest pp(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"], "box");
		//std::shared_ptr <testInstance> pp2 = std::make_shared<testInstance>(pp);
		
		objectsTEST.emplace_back(std::make_shared<testInstance>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"], "box"));
		//objectsTEST.push_back(pp2);
		//objects2.emplace_back(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"], "box");
		// uniforms
		unsigned int texture1, texture2;
		//

		//objects[index].createTexture("container.jpg", texture1);
		//objects[index].createTexture("awesomeface.png", texture2, true);
	}

	void createObjects() {
		createBoxes();
		createTorus();

	}

	void createShaders() {
		shaders.emplace("basic", std::make_shared<Shader>(".\\shaders\\vertex_shader.txt", ".\\shaders\\fragment_shader.txt"));
		shaders.emplace("basic2", std::make_shared<Shader>(".\\shaders\\vertex_shader.txt", ".\\shaders\\fragment_shader.txt"));
	}

	void setViewAndProjection(std::shared_ptr <Shader> shader) {
		glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = Wonderland::camera.GetViewMatrix();
		shader->setMat4("view", view);
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
		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS)
			camera.ProcessKeyboard(STATIC1, delta_time);
		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS)
			camera.ProcessKeyboard(STATIC2, delta_time);
	
	}
};

