#include <iostream>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h> //GL Extenton Wrangler provides runtime support for determining extentions to use on target platform
#include <GLFW/glfw3.h> //GLFW is the library for using OpenGL in a windowed context
#include "Shader.h"
#include <glm/glm.hpp> //glm is a math library for matrix math
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
using namespace glm;

struct shape {
	std::vector<vec3> vertices;
	GLuint VAO;
	GLuint VBO;

	vec3 scale = vec3(1.f, 1.f, 1.f);
	vec3 rotation = vec3(0.5f, 0.78f, 0.f);
	vec3 position = vec3(0.5f, 0.f, 0.f);
	mat4 worldMatrix;

	GLuint IBO; //IndexBufferObject
	std::vector<GLushort> indicies;
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
	shape quad;
	quad.vertices = {
		vec3(-0.5f, -0.5f, 0.0f),	vec3(1.0f, 0.0f, 0.0f), // Vertex 0
		vec3(0.5, -0.5f, 0.0f),		vec3(0.0f, 1.0f, 0.0f), // Vertex 1
		vec3(-0.5f, 0.5f, 0.0f),  	vec3(0.0f, 0.0f, 1.0f), // Vertex 2
		vec3(0.5f, 0.5f, 0.0f),  	vec3(1.0f, 0.0f, 0.0f)  // Vertex 3
	};
	quad.indicies = {
		0,1,2,
		2,1,3
	};

	// =Vertex Buffer Objects contain only the data for vertices=
	// =Vertex Array Objects contain details and format for those vertices=
	glGenVertexArrays(1, &quad.VAO); //creating vertex array
	glBindVertexArray(quad.VAO); //set current VertexArray to object's VAO
	//(target, sizeOfBuffer, start address, usage)
	//(howManyElementsInArray, addressOfArray)
	glGenBuffers(1, &quad.VBO); //creating vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, quad.VBO); //sets the current ArrayBuffer to object's VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * quad.vertices.size(), &quad.vertices[0], GL_STATIC_DRAW); //describes data properies of the VBO
	glEnableVertexAttribArray(0); //enables the VertexAttribArray with the index 0
	//(index, numOfElementsInArray, typeOfElements, normalized aka [0, 1], sizeOfArray, offset??)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), 0); //defines an array of vertex attribute data for index 0
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (GLvoid*)(sizeof(vec3))); //this vertex attribute is for color

	glGenBuffers(1, &quad.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * quad.indicies.size(), &quad.indicies[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbinds the array buffer (0 means unbind)
	glBindVertexArray(0); //unbind vertex array object

	Shader basicShader;
	basicShader.load();
	basicShader.use();

	// === GAME LOOP ===
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); //check for events

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //specifies the RGBA values which will be used by glClear to clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT); //clearing the buffer to be the background color

		quad.worldMatrix =
			translate(quad.position) *
			yawPitchRoll(quad.rotation.y, quad.rotation.x, quad.rotation.z) *
			scale(quad.scale);

		//(location, count, transpose, valueLocation)
		glUniformMatrix4fv(2, 1, GL_FALSE, &quad.worldMatrix[0][0]); //Assigns a 4x4 float matrix to uniform
		//why 2?

		// === DRAWING ===
		glBindVertexArray(quad.VAO);
		//(mode, firstIndex, count)
		//glDrawArrays(GL_TRIANGLES, 0, quad.vertices.size());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad.IBO);
		//(mode, count, typeOfIndex, pointerToWhereIndiciesAre)
		glDrawElements(GL_TRIANGLES, quad.indicies.size(), GL_UNSIGNED_SHORT, (void*)0); //0 assumes loaded array buffer
		glBindVertexArray(0); //unbind

		glfwSwapBuffers(window); //swap buffers at the very end after finished rendering
	}


	glDeleteVertexArrays(1, &quad.VAO);
	glDeleteBuffers(1, &quad.VBO);
	glfwTerminate();
	return EXIT_SUCCESS;
}