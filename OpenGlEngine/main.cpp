#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h> //GL Extenton Wrangler provides runtime support for determining extentions to use on target platform
#include <GLFW/glfw3.h> //GLFW is the library for using OpenGL in a windowed context

const GLint WIDTH = 800, HEIGHT = 600;


int main()
{
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

	// This allows GLEW take the modern approach to retrive function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup OpenGL function pointers
	if (GLEW_OK != glewInit())
	{
		// If the initalization is not successful, print out the message and exit the program with return value EXIT_FAILURE
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	//setting up viewport
	//(top, left, width, height)
	glViewport(0, 0, screenWidth, screenHeight);




	//Game Loop! Runs until window is closed
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents(); //check for events

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //specifies the RGBA values which will be used by glClear to clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT); //clearing the buffer


		glfwSwapBuffers(window); //swap buffers at the very end after finished rendering
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}