#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.hpp"
#include "Shader.h"


int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Modern OpenGL", nullptr, nullptr);

	int screenWidth = 0, screenHeight = 0;
	
	// Retina displays
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

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
	
	// Define viewport dimensions
	glViewport( 0, 0, screenWidth, screenHeight );

	Shader ourShader( "core.vs", "core.fs" );

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	// left + red
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	// right + blue
 		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f		// top + green
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( void * )( 3* sizeof(GLfloat) ));
	glEnableVertexAttribArray( 1 );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// render
		glClearColor(0.2f, 0.3f, 0.0f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		//draw opengl
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();


	return EXIT_SUCCESS;
}
