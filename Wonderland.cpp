#include "Wonderland.h"


int Wonderland::initEnviroment()
{
	camera = Camera();
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
	glfwSetFramebufferSizeCallback(win, &Wonderland::windowResizeCallback);
	glfwSetCursorPosCallback(win, &Wonderland::mouseCallback);
	glfwSetScrollCallback(win, &Wonderland::scrollCallback);

	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//initialize GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);
}

void Wonderland::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
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

void Wonderland::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void Wonderland::windowResizeCallback(GLFWwindow* win, int width, int height)
{
	glViewport(0, 0, width, height);
}
