#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include "ConstsAndTypes.h"
#include <iostream>
#include "Camera.h"
#include "Objects.h"
#include "ObjectBase.h"
#include "Skybox.h"
#include <map>
#include "Model.h"


namespace Wonderland {
	Camera camera;
	float lastX;
	float lastY;
	bool firstMouse;
	float delta_time;
	float last_frame;
	GLFWwindow* win;
	bool flashlight_on = false;
	bool fog = false;
	std::vector <std::shared_ptr<ObjectBase>> scene_objects;
	std::map <std::string, std::shared_ptr<Shader>> shaders;
	std::map <int, bool> key_pressed;
	std::unique_ptr <Skybox> skybox;
	//std::unique_ptr <Model> uniq_model;

	void toggleFlashlight() {
		flashlight_on = !flashlight_on;
	}

	void toggleFog() {
		fog = !fog;
	}

	void createObjects() {
		size_t index = scene_objects.size();

		scene_objects.emplace_back(std::make_shared <YellowBox>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <Plane>(planeVertices, planeNVertices * 8, planeTriangles, planeNTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <Lava>(lavaVertices, lavaNVertices * 8, lavaTriangles, lavaNTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <Mushrooms>(cylinderVertices, cylinderNVertices * 8, cylinderTriangles, cylinderNTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <God>(torus_001Vertices, torus_001NVertices * 8, torus_001Triangles, torus_001NTriangles, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <LightBlueBox>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["basic"]));
		scene_objects.emplace_back(std::make_shared <LightSource>(kukuVert, kukuN * 8, kukuTri, kukuTriCNT, shaders["light"], shaders["basic"]));
		
		// uniforms
		unsigned int texture1, texture2;


		std::vector <std::string> skybox_faces { ".\\skybox\\right.jpg", ".\\skybox\\left.jpg", ".\\skybox\\top.jpg",
										  ".\\skybox\\bottom.jpg", ".\\skybox\\front.jpg", ".\\skybox\\back.jpg"};
		skybox = std::make_unique<Skybox>(skybox_faces, skyboxVertices, 108, shaders["skybox"]);
		shaders["basic"]->setBool("is_lava", false);
		//uniq_model = std::make_unique <Model> (".\\objects\\backpack.obj");
		//objects[index].createTexture("container.jpg", texture1);
		//objects[index].createTexture("awesomeface.png", texture2, true);
	}


	void createShaders() {
		shaders.emplace("basic", std::make_shared<Shader>(".\\shaders\\vertex_shader.vert", ".\\shaders\\fragment_shader.frag"));
		shaders.emplace("light", std::make_shared<Shader>(".\\shaders\\vs_light.vert", ".\\shaders\\fs_light.frag"));
		shaders.emplace("skybox", std::make_shared<Shader>(".\\shaders\\skybox.vert", ".\\shaders\\skybox.frag"));
		shaders.emplace("multimesh", std::make_shared<Shader>(".\\shaders\\multimesh_obj.vert", ".\\shaders\\multimesh_obj.frag"));
		
	}


	void drawMultimesh() {
		std::shared_ptr <Shader> shader = shaders["multimesh"];
		shader->use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(12.0f, 30.0f, 5.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		shader->setMat4("model", model);
		//uniq_model->Draw(shader);
	}


	void setSkyboxMatrices() {
		std::shared_ptr <Shader> shader = shaders["skybox"];
		shader->use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) WIN_WIDTH / (float) WIN_HEIGHT, 0.1f, 100.0f);

		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
	}

	void setViewAndProjection(std::shared_ptr <Shader> shader) {
		std::shared_ptr <Shader> current_shader = shaders["basic"];
		current_shader->use();
		// setup flashlight
		current_shader->setVec3("flashlight.position", camera.Position);
		//shaders["basic"]->setVec3("light_pos_unif", camera.Position);
		//shaders["basic"]->setVec3("flashlight.position", glm::vec3(0.0f, 0.0f, 0.0f));
		current_shader->setVec3("flashlight.direction", camera.Front);
		current_shader->setFloat("flashlight.cut_off", glm::cos(glm::radians(10.0f)));
		float epsilon = flashlight_on ? 17.0f : 0.0f;
		current_shader->setFloat("flashlight.outer_cut_off", glm::cos(glm::radians(epsilon)));
		current_shader->setInt("fog", fog);


		if (current_shader != shader) shader->use();
		shader->setVec3("view_pos", camera.Position);
		glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = Wonderland::camera.GetViewMatrix();
		shader->setMat4("view", view);
		shader->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

		// set time seed in shader
		shader->setFloat("time_seed", glfwGetTime()/20.0);
		//std::cout << "glfw time seed: " << (int)(glfwGetTime() * 1000) / 100 << " and undivided: " << glfwGetTime() << std::endl;
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
		key_pressed.emplace(GLFW_KEY_F, false);
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
		if (glfwGetKey(win, GLFW_KEY_F) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F] = false;
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
		if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS && !key_pressed[GLFW_KEY_F]) {
			toggleFog();
			key_pressed[GLFW_KEY_F] = true;
		}
		
	}
};

