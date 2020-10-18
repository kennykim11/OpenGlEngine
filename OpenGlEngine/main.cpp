#include <iostream>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> //GL Extenton Wrangler provides runtime support for determining extentions to use on target platform
#include <GLFW/glfw3.h> //GLFW is the library for using OpenGL in a windowed context
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

struct shape {
	std::vector<vec3> vertices;
	GLuint VAO;
	GLuint VBO;
};

const GLint WIDTH = 800, HEIGHT = 600;


int main()
{
	// === SETUP ===
	glfwInit(); //Initialize
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Set to version 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using the Core profile instead of Compat (for backwards-compatibility)
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Setting forward compatibility to true
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Right now, window not resizable

	//(width, height, windowTitle, GLFWmonitor for fullscreen, GLFWWindow to use context of existing window)
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Opening a window", nullptr, nullptr);
	//Getting the actual window size, useful for accomidating high-density screens
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	// Check if the window creation was successful by checking if the window object is a null pointer or not
	if (window == nullptr)
	{
		// If the window returns a null pointer, meaning the window creation was not successful
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window); //setting our new window to be the current context
	glewExperimental = GL_TRUE; //use modern magic for glew
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	//setting up viewport
	//(top, left, width, height)
	glViewport(0, 0, screenWidth, screenHeight);



	// === OBJECTS ===
	shape triangle;
	triangle.vertices = {
		vec3(-1.0f, -0.5f, 0.0f),	vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, -0.5f, 0.0f),	vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.0f),	vec3(0.0f, 0.0f, 1.0f)
	};

	// =Vertex Buffer Objects contain only the data for vertices=
	// =Vertex Array Objects contain details and format for those vertices=
	//(howManyElementsInArray, addressOfArray)
	glGenVertexArrays(1, &triangle.VAO); //creating vertex array
	glGenBuffers(1, &triangle.VBO); //creating vertex buffer
	glBindVertexArray(triangle.VAO); //set current VertexArray to VAO
	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO); //sets the current ArrayBuffer to VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * triangle.vertices.size(), &triangle.vertices[0], GL_STATIC_DRAW); //describes data properies of the VBO
	glEnableVertexAttribArray(0); //enables the VertexAttribArray with the index 0
	//(index, numOfElementsInArray, typeOfElements, normalized aka [0, 1], sizeOfArray, offset??)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (GLvoid*)(sizeof(vec3))); //defines an array of vertex attribute data for index 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (GLvoid*)(sizeof(vec3))); //this vertex attribute is for color
	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinds the array buffer (0 means unbind)
	glBindVertexArray(0); //unbind vertex array object

	Shader basicShader;
	basicShader.load();

	// === GAME LOOP ===
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); //check for events

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //specifies the RGBA values which will be used by glClear to clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT); //clearing the buffer to be the backgroun color

		// === DRAWING ===
		basicShader.use();
		glBindVertexArray(triangle.VAO);
		//(mode, firstIndex, count)
		glDrawArrays(GL_TRIANGLES, 0, triangle.vertices.size());
		glBindVertexArray(0); //unbind

		glfwSwapBuffers(window); //swap buffers at the very end after finished rendering
	}


	glDeleteVertexArrays(1, &triangle.VAO);
	glDeleteBuffers(1, &triangle.VBO);
	glfwTerminate();
	return EXIT_SUCCESS;
}