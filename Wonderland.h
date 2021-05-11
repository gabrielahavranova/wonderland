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
	bool picking_on = false;
	bool getClickLocation = false;
	std::vector <std::shared_ptr<ObjectBase>> simple_scene_objects;
	std::map <std::string, std::shared_ptr<Shader>> shaders;
	std::map <int, bool> key_pressed;
	std::map <int, std::shared_ptr<ObjectBase>> clickable_objects;
	std::unique_ptr <Skybox> skybox;
	std::vector <glm::vec3> colliders;
	std::map <std::string, std::shared_ptr<Model>> mm_scene_objects;
	//std::vector <Model> multimesh_scene_objects;

	void toggleFlashlight() {
		flashlight_on = !flashlight_on;
	}

	void toggleFog() {
		fog = !fog;
	}

	void togglePicking() {
		picking_on = !picking_on;
	}

	void createObjects() {
		size_t index = simple_scene_objects.size();

		simple_scene_objects.emplace_back(std::make_shared <Mush>(mushroomVertices, mushroomNVertices* 8, mushroomTriangles, mushroomNTriangles, shaders["basic"], "mushtex.png", colliders));
		simple_scene_objects.emplace_back(std::make_shared <BlueShrooms>(mushroom1Vertices, mushroom1NVertices * 8, mushroom1Triangles, mushroom1NTriangles, shaders["basic"], "Mtex.png", colliders));
		for (int i = 1; i < 5; i++) clickable_objects.emplace(i, simple_scene_objects.back());
		simple_scene_objects.emplace_back(std::make_shared <Lava>(newlavaVertices, newlavaNVertices * 8, newlavaTriangles, newlavaNTriangles, shaders["basic"], 0xFF));
		clickable_objects.emplace(0xFF, simple_scene_objects.back());
		simple_scene_objects.emplace_back(std::make_shared <Mushrooms>(mushroom2Vertices, mushroom2NVertices * 8, mushroom2Triangles, mushroom2NTriangles, shaders["basic"], "mushroom2tex.png", colliders));
		for (int i = 10; i < 17; i++) clickable_objects.emplace(i, simple_scene_objects.back());
		//simple_scene_objects.emplace_back(std::make_shared <Mushrooms>(cylinderVertices, cylinderNVertices * 8, cylinderTriangles, cylinderNTriangles, shaders["basic"], colliders));
		simple_scene_objects.emplace_back(std::make_shared <God>(suzanneVertices, suzanneNVertices * 8, suzanneTriangles, suzanneNTriangles, shaders["basic"]));
		//simple_scene_objects.emplace_back(std::make_shared <LightBlueBox>(cubeVertices, cubeNVertices * 8, cubeTriangles, cubeNTriangles, shaders["basic"]));
		simple_scene_objects.emplace_back(std::make_shared <Stars>(starVertices, starNVertices * 8, starTriangles, starNTriangles, shaders["light"]));
		simple_scene_objects.emplace_back(std::make_shared <LightSource>(cubeVertices, cubeNVertices * 8, cubeTriangles, cubeNTriangles, shaders["light"], shaders["basic"]));

		
		// uniforms
		unsigned int texture1, texture2;


		std::vector <std::string> skybox_faces { ".\\skybox\\right.png", ".\\skybox\\left.png", ".\\skybox\\top.png",
										  ".\\skybox\\bottom.png", ".\\skybox\\front.png", ".\\skybox\\back.png"};
		skybox = std::make_unique<Skybox>(skybox_faces, skyboxVertices, 108, shaders["skybox"]);
		simple_scene_objects.emplace_back(std::make_shared <Flame>(flameVertices, flameNVertices * 8, flameTriangles, flameNTriangles, shaders["basic"]));
		shaders["basic"]->use();
		shaders["basic"]->setBool("is_lava", false);
		shaders["basic"]->setBool("is_flame", false);
		shaders["basic"]->setBool("is_flame_frag", false);
		mm_scene_objects.emplace("moon", std::make_shared<Model>(".\\objects\\moon.obj"));
		mm_scene_objects.emplace("plane", std::make_shared<Model>(".\\objects\\plane5.obj"));
		//mm_scene_objects.emplace("eyeball", std::make_shared<Model>(".\\objects\\eyeball.obj"));
		//uniq_model = std::make_unique <Model> (".\\objects\\backpack.obj");
		//objects[index].createTexture("container.jpg", texture1);
		//objects[index].createTexture("awesomeface.png", texture2, true);
		std::cout << " distance is: " << glm::distance(glm::vec3(-27.9156f, 10.0f, -56.3888f), glm::vec3(6.36411f, 10.0f, -15.9944f)) << std::endl;
	
		colliders.emplace_back(-27.9f, -56.38, 59.0f);
	}

	void createShaders() {
		shaders.emplace("basic", std::make_shared<Shader>(".\\shaders\\vertex_shader.vert", ".\\shaders\\fragment_shader.frag"));
		shaders.emplace("light", std::make_shared<Shader>(".\\shaders\\vs_light.vert", ".\\shaders\\fs_light.frag"));
		shaders.emplace("skybox", std::make_shared<Shader>(".\\shaders\\skybox.vert", ".\\shaders\\skybox.frag"));
		shaders.emplace("multimesh", std::make_shared<Shader>(".\\shaders\\multimesh_obj.vert", ".\\shaders\\multimesh_obj.frag"));
		
	}

	void drawEyeball(std::shared_ptr<Model> eyeball, std::shared_ptr<Shader>shader) {
		glm::mat4 model = glm::mat4(1.0f);
		shaders["multimesh"]->use();

		model = glm::translate(model, glm::vec3(0.0f, 5.5f, 0.0f));
		//float angle = -90.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		shaders["multimesh"]->setMat4("model", model);
		shaders["multimesh"]->setFloat("color_intensity", 0.8f);
		//setModelMatrices(model);
		//setMeshMaterial(glm::vec3(0.01f, 0.01f, 0.01f), 0.0f);
		// --------------------------v  = indices !!!! CNT !!!! FKING HELL!!!!!! 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		eyeball->Draw(shaders["multimesh"]);
	
	}

	void drawPlane(std::shared_ptr<Model> plane, std::shared_ptr<Shader>shader) {
		glm::mat4 model = glm::mat4(1.0f);
		shaders["basic"]->use();
		model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
		
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, 0.0f));
		//float angle = -90.0f;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f, 8.5f, 8.5f));
		shaders["basic"]->setMat4("model", model);
		shaders["basic"]->setFloat("color_intensity", 0.2f);
		//setModelMatrices(model);
		//setMeshMaterial(glm::vec3(0.01f, 0.01f, 0.01f), 0.0f);
		// --------------------------v  = indices !!!! CNT !!!! FKING HELL!!!!!! 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		plane->Draw(shaders["basic"]);
	} 

	void drawMoon(std::shared_ptr <Model> moon, std::shared_ptr<Shader> shader) {
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, NEAR_P, FAR_P * 3);
		glm::mat4 view = camera.GetViewMatrix();
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		shader->setFloat("color_intensity", 2.0f);
		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(12.0f, 80.0f, 5.0f));
		double time = glfwGetTime();
		float time_seed = (int)(time * 100) / 100.0f;
		float an = 0.02f;
		glm::vec3 moon_pos(12.0f + glm::cos(an * time_seed) * 140.0f, 500.0f, 5.0f + glm::sin(an * time_seed) * 140.0f);
		model = glm::translate(model, moon_pos);
		//shaders["basic"]->use();
		//shaders["basic"]->setVec3("dir_light.direction", moon_pos);
		//std::cout << "moon position is: " << moon_pos.x << " " << moon_pos.y << " " << moon_pos.z << std::endl;
		shader->use();
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -an * time_seed, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(100.0f, 100.0f, 100.0f));
		shader->setMat4("model", model);
		moon->Draw(shader);
	}

	void drawMultimesh() {
		std::shared_ptr <Shader> shader = shaders["multimesh"];
		shader->use();

		for (auto it = mm_scene_objects.begin(); it != mm_scene_objects.end(); it++) {
			if (it->first == "moon") drawMoon(it->second, shader);
			else if (it->first == "plane") drawPlane(it->second, shader);
			else if (it->first == "eyeball") drawEyeball(it->second, shader);
			else {}

		}
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
		current_shader->setVec3("flashlight.strength", glm::vec3(0.3f, 0.3f, 0.3f));
		current_shader->setFloat("flashlight.cut_off", glm::cos(glm::radians(10.0f)));
		float epsilon = flashlight_on ? 17.0f : 0.0f;
		current_shader->setFloat("flashlight.outer_cut_off", glm::cos(glm::radians(epsilon)));


		current_shader->setVec3("picking.position", camera.Position);
		//shaders["basic"]->setVepickingpos_unif", camera.Position);
		//shaders["basic"]->setVepickingight.position", glm::vec3(0.0f, 0.0f, 0.0f));
		current_shader->setVec3("picking.direction", camera.Front);
		current_shader->setFloat("picking.cut_off", glm::cos(glm::radians(0.2f)));
		epsilon = 0.5f;
		current_shader->setFloat("picking.outer_cut_off", glm::cos(glm::radians(epsilon)));
		current_shader->setInt("picking.on", picking_on);
		current_shader->setFloat("lava_sped", 1.0f);
		current_shader->setInt("fog", fog);
		


		if (current_shader != shader) shader->use();
		shader->setVec3("view_pos", camera.Position);
		glm::mat4 projection = glm::perspective(glm::radians(Wonderland::camera.Zoom), (float)WIN_WIDTH / (float)WIN_HEIGHT, 0.05f, FAR_P*3);
		shader->setMat4("projection", projection);

		// camera/view transformation
		glm::mat4 view = Wonderland::camera.GetViewMatrix();
		shader->setMat4("view", view);
		shader->setVec3("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

		double time = glfwGetTime();
		int time_i = (int)(time * 100) / 5;

		// set time seed in shader
		shader->setFloat("time_seed", time / 20.0);
		shader->setInt("flame.x_offset", time_i % 4);
		shader->setInt("flame.y_offset", time_i / 4);
		//std::cout << "glfw time seed x: " << time;
		//std::cout << "time x 100 double:" << time * 100;
		//std::cout << "time x 100 int:" << (int)(time * 100) / 10 << std::endl;
		
	}

	void makeClickAction(unsigned char color[4]) {
		int object_id = 0;
		if (color && color[2]) object_id = (int)color[2];
		else if (color && color[0]) object_id = (int)color[0];
		else if (color && color[1]) object_id = (int)color[1];
		else return;

		auto it = clickable_objects.find(object_id);
		if (it != clickable_objects.end()) {
			it->second->applyClick(object_id);
		}
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

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !key_pressed[GLFW_MOUSE_BUTTON_LEFT]) {
			key_pressed[GLFW_MOUSE_BUTTON_LEFT] = true;
			shaders["basic"]->use();
			shaders["basic"]->setBool("click_test.perform", true);
			getClickLocation = true;
			std::cout << "down" << std::endl;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			key_pressed[GLFW_MOUSE_BUTTON_LEFT] = false;
			shaders["basic"]->use();
			shaders["basic"]->setBool("click_test.perform", false);
			std::cout << "mouse up " << std::endl;
		}
	}

	void windowResizeCallback(GLFWwindow* win, int width, int height) {
		glViewport(0, 0, width, height);
	}

	int initEnviroment() {
		camera = Camera(glm::vec3(98.1605f, 10.001f, -47.9002f));
		lastX = WIN_WIDTH / 2.0f;
		lastY = WIN_HEIGHT / 2.0f;
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
		glfwSetMouseButtonCallback(win, mouseButtonCallback);
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
		key_pressed.emplace(GLFW_KEY_P, false);
		key_pressed.emplace(GLFW_MOUSE_BUTTON_LEFT, false);
	}

	void processInput() {
		if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(win, true);
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, delta_time, colliders);
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, delta_time, colliders);
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, delta_time, colliders);
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, delta_time, colliders);
		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F1] = false;
		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F2] = false;
		if (glfwGetKey(win, GLFW_KEY_L) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_L] = false;
		if (glfwGetKey(win, GLFW_KEY_F) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_F] = false;
		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS && !key_pressed[GLFW_KEY_P]) {
			key_pressed[GLFW_KEY_P] = true;
			togglePicking();
		}
		if (glfwGetKey(win, GLFW_KEY_P) == GLFW_RELEASE)
			key_pressed[GLFW_KEY_P] = false;

		if (glfwGetKey(win, GLFW_KEY_F1) == GLFW_PRESS && !key_pressed[GLFW_KEY_F1]) {
			camera.ProcessKeyboard(STATIC1, delta_time, colliders);
			key_pressed[GLFW_KEY_F1] = true;
		}
		if (glfwGetKey(win, GLFW_KEY_F2) == GLFW_PRESS && !key_pressed[GLFW_KEY_F2]) {
			camera.ProcessKeyboard(STATIC2, delta_time, colliders);
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

