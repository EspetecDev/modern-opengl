#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.hpp"


int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modern OpenGL", nullptr, nullptr);

	int screenWidth, screenHeight;
	
	// Retina displays
	glfwGetFramebufferSize(window, &screenHeight, &screenHeight);

	if (window == nullptr) {
		
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {

		std::cout << "Failed to init GLEW" << std::endl;

		return EXIT_FAILURE;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// render
		glClearColor(0.2f, 0.3f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw opengl
		glfwSwapBuffers(window);
	}



	std::cout << "Hello World" << std::endl;


	return EXIT_SUCCESS;
}
