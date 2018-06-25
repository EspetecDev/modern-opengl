#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.hpp"
#include "Shader.h"

#include "SOIL2\SOIL2.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


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

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	Shader ourShader( "core.vs", "core.fs" );

	GLfloat vertices[] = {
		// Positions		// Colors			// Texture Coordinates
		0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// Top rigth
		0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,		// Bottom right
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,		// Bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f		// Top left
	};
	
	// Indexing vertices (sharing common vertices)
	GLuint indices[] =
	{
		0, 1, 3,	// First triangle
		1, 2, 3		// Second triangle
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers( 1, &EBO );

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )(3* sizeof(GLfloat) ));
	glEnableVertexAttribArray( 1 );
	// Texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( GLfloat ), ( GLvoid * )(6 * sizeof( GLfloat )) );
	glEnableVertexAttribArray( 2 );

	// Unbind VAO
	glBindVertexArray(0);

	GLuint texture;
	int width, height;

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	std::cout << "Loading texture: " << std::endl;
	unsigned char *image = SOIL_load_image( "../src/assets/fuet.jpg", &width, &height, 0, SOIL_LOAD_RGBA );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image );
	glGenerateMipmap( GL_TEXTURE_2D );
	SOIL_free_image_data( image );
	glBindTexture( GL_TEXTURE_2D, 0 );

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		ourShader.Use();

		/* TODO: fix it */
		// Transformation
		glm::mat4 transform = glm::mat4( 1.0f);
		transform = glm::translate( transform, glm::vec3( 0.0f, 0.0f, 0.0f ) );
		transform = glm::rotate( transform, (GLfloat) glfwGetTime() * -5.0f, glm::vec3( 0.0f, 0.0f, 1.0f));
		
		
		GLint transformationLocation = glGetUniformLocation( ourShader.Program, "transform" );
		glUniformMatrix4fv(transformationLocation, 1, GL_FALSE, glm::value_ptr(transform));


		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture );
		glUniform1i( glGetUniformLocation( ourShader.Program, "ourTexture" ), 0 );

		glBindVertexArray(VAO);
		glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray(0);

		//draw opengl
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers( 1, &EBO );

	glfwTerminate();


	return EXIT_SUCCESS;
}
