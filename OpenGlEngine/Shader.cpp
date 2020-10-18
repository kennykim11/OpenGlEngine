/*
File: Shader.cpp
Author: Sanketh Bhat
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


//This loads GLSL shaders for our use
//Feel free to just copy the header and source files and use them
//Make sure you know whats going on in the file, but no need to remember how to replicate it.
//Only handles vertex shaders and fragment(pixel) shaders.
#include "Shader.h"



Shader::Shader()
{
}

//Overloaded constructor, can be used when we want to use shaders other than the default ones.
Shader::Shader(std::string fnv, std::string fnf)
{
	filenamev = fnv;
	filenamef = fnf;
}


Shader::~Shader()
{
}

bool Shader::load()
{
	//Storing our compliation results for debuggi ng
	bool resultV = compile(GL_VERTEX_SHADER);
	bool resultF = compile(GL_FRAGMENT_SHADER);

	//If either shader failed to comple, return false
	if (resultV == false || resultF == false)
		return false;

	//Attaching shaders to the program
	program = glCreateProgram();		// Creating a base OpenGL program
	glAttachShader(program, vprogram);	// Attaching the compiled vertex shader
	glAttachShader(program, fprogram);	// Attaching the compiled frag shader
	glLinkProgram(program);				// Links shader program object so we can access it later

	//Debugging Program
	GLint checkStatus = 0;

	//This line returns a parameter from a shader program object, specified by the second parameter
	glGetProgramiv(program, GL_LINK_STATUS, &checkStatus);	//Check if the shader program linked correctly, stores result in the last parameter. Second 

	//If it's false, it didn't link properly so we have to debug it
	if (checkStatus == GL_FALSE)
	{
		GLint length = 0;
		//Same line as above, except this time we want the log data length so we can output it
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* logData = new GLchar[length + 1];

		//This line stores the information log from the shader program into a specified variable.
		glGetProgramInfoLog(program, length, 0, logData);
		std::cout << logData << "\n";

		//Since we failed to create the shader program, delete any created objects
		glDeleteProgram(program);
		delete[] logData;

		//Print an error messavge and return
		std::cout << "Error linking program file\n";
		return false;
	}

	return true;
}

bool Shader::compile(GLenum shaderType)
{
	std::ifstream inFile;
	GLint checkStatus;

	//Find the GLSL file, second paramter specifies to open the file in binary format
	
	// If we are compiling the vertex shader
	if (shaderType == GL_VERTEX_SHADER)
	{
		// reminder:
		// filenamev was set in the 
		// shader class constructor
		
		// load the vertex shader file
		// notice the 'v' in filenameV
		inFile.open(filenamev, std::ios::binary);
	}
	
	// If we are compiling the fragment shader
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		// reminder:
		// filenamef was set in the 
		// shader class constructor
		
		// load the fragment shader file
		// notice the 'f' in filenamef
		inFile.open(filenamef, std::ios::binary);
	}

	//If we managed to open the file, continue
	if (inFile.is_open())
	{
		//Getting binary file length
		inFile.seekg(0, std::ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(0, std::ios::beg);

		//Reading contents
		char* fileContents = new char[length + 1];
		inFile.read(fileContents, length);

		//Null pointer
		fileContents[length] = '\0';

		// close the file
		inFile.close();

		// create an empty shader program piece,
		// with the type we want
		// A full shader program made after compiling many
		// of these pieces (vertex and fragment)
		GLuint shaderProgramPiece = glCreateShader(shaderType);

		// give the shader source to our empty shader program
		glShaderSource(shaderProgramPiece, 1, &fileContents, 0);

		// now that our program has source code
		// compile the program
		glCompileShader(shaderProgramPiece);
		
		// get the status to see if it compiles correctly
		glGetShaderiv(shaderProgramPiece, GL_COMPILE_STATUS, &checkStatus);

		// if the status is zero, then the compile failed
		if (checkStatus == 0)
		{
			// set length to zero by default
			GLint length = 0;

			// this gives us the length of the error message(s)
			// that the compiler found when trying to compile our code
			glGetShaderiv(shaderProgramPiece, GL_INFO_LOG_LENGTH, &length);

			// create a string that is large enough to store the
			// error message(s)
			GLchar* logData = new GLchar[length + 1];

			// get the error message(s) and store them into our string
			glGetShaderInfoLog(shaderProgramPiece, length, 0, logData);

			// print the data, so that we can see the error
			std::cout << logData << "\n";

			// delete the program, because it failed, it is 
			// not useful to us in any way
			glDeleteShader(shaderProgramPiece);

			// delete the error message, we already printed it,
			// so we dont need it here anymore
			delete[] logData;

			// return false, because the program did not
			// do what we wanted it to do
			return false;

		}

		// if the status is not zero, then the shader
		// compiled successfully
		else
		{
			// if we are processing a vertex shader
			if (shaderType == GL_VERTEX_SHADER)
			{
				// save the vprogram (vertex shader program)
				// as the current program we are processing
				vprogram = shaderProgramPiece;
			}
				
			// if we are processing a fragment shader
			else if (shaderType == GL_FRAGMENT_SHADER)
			{
				// save the fprogram (vertex shader program)
				// as the current program we are processing
				fprogram = shaderProgramPiece;
			}
			
			// delete the original source code from RAM,
			// because we don't need it anymore. Of course,
			// this does not erase the file on the hard drive,
			// just the contents of the file that we loaded
			delete[] fileContents;

			// return true, because the function ran successfully.
			// This, of course, ends the function
			return true;
		}
	}

	// If we were unable to open the file
	// that we tried to open
	else
	{
		std::cout << "Error opening shader file\n";

		// return false, because the program
		// did not do what we wanted it to do
		return false;
	}
}

void Shader::use()
{
	//Specifies what shader program to use while rendering
	glUseProgram(program);
}

//Delete created shader programs
void Shader::unload()
{
	glDeleteProgram(program);
	glDeleteShader(vprogram);
	glDeleteShader(fprogram);

	program = 0;
	vprogram = 0;
	fprogram = 0;
}
