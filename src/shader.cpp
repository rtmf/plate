/* vim: set ai noet ts=4 sts=4 sw=4: */
#include "shader.h"
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
using namespace std;

template <GLenum SHADER_TYPE> ShaderObject<SHADER_TYPE>::ShaderObject(string filename) :
	filename(filename),
	shaderObject(0)
{
	try
	{
		/* Grab filename I guess */
		this->filename = filename;

		/* Create OpenGL Shader Object */
		shaderObject = glCreateShader(SHADER_TYPE);
		if (shaderObject == 0)
			throw ShaderBuildException("glCreateShader() = 0");

		/* Grab shader source code from disk */
		ifstream shaderSourceStream(filename.c_str());
		if (!shaderSourceStream.is_open())
		{
			string errorMessage("Could not open file \"" + filename + "\"");
			throw ShaderBuildException(errorMessage);
		}
		string shaderSourceString(
			(istreambuf_iterator<char>(shaderSourceStream)),
			istreambuf_iterator<char>());
		
		/* Give OpenGL the source code for the shader */
		const GLchar* shaderSourceAddress = (GLchar*) shaderSourceString.c_str();
		GLint shaderSourceLength = (GLint) shaderSourceString.length();
		glShaderSource(shaderObject, 1, &shaderSourceAddress, &shaderSourceLength);

		/* Compile the code */
		glCompileShader(shaderObject);
		GLint iResult;
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &iResult);
		if (iResult == GL_FALSE)
		{
			/* Compilation failed; request "info log" for compiler error details */
			/* Begin with fetching size of "info log" */
			glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &iResult);
			/* No info log! */
			if (iResult <= 1)
				throw ShaderBuildException("glCompileShader() failed silently");
			/* Allocate memory for info log */
			char* infoLogBuffer = new char[iResult];
			/* Retrieve info log from GL */
			glGetShaderInfoLog(shaderObject, iResult, NULL, infoLogBuffer);
			/* Throw */
			string errorMessage(infoLogBuffer);
			delete infoLogBuffer;
			throw ShaderBuildException(errorMessage);
		}
	}
	catch (...)
	{
		releaseGLStuff();
		throw;
	}
}

template <GLenum SHADER_TYPE> void ShaderObject<SHADER_TYPE>::releaseGLStuff()
{
	if (shaderObject != 0)
	{
		glDeleteShader(shaderObject);
		shaderObject = 0;
	}
}

template <GLenum SHADER_TYPE> ShaderObject<SHADER_TYPE>::~ShaderObject()
{
	releaseGLStuff();
}

ShaderProgram::ShaderProgram(string vertexShaderFilename, string fragmentShaderFilename) :
	vertexShaderObject(NULL),
	fragmentShaderObject(NULL),
	programObject(0)
{
	try
	{
		build(vertexShaderFilename, vertexShaderObject,
			  fragmentShaderFilename, fragmentShaderObject, programObject);
		/* Everything worked! Free our old resources and take up the new ones. */
		this->vertexShaderObject = vertexShaderObject;
		this->fragmentShaderObject = fragmentShaderObject;
		this->programObject = programObject;
	}
	catch (...)
	{
		releaseGLStuff();
		throw;
	}
}

ShaderProgram::~ShaderProgram()
{
	releaseGLStuff();
}

bool ShaderProgram::rebuild()
{
	/* Create shader objects */
	try
	{
		/* Temporary holding places for values, in case we error and need to rely
		 * on our old program.
		 */
		VertexShaderObject* vertexShaderObject;
		FragmentShaderObject* fragmentShaderObject;
		GLuint programObject;

		/* Reload shader source, compile, and link. If any error occurs, catch it
		 * and report it to stderr instead of propagating the exception. We still
		 * have our original shader program in working order, so this doesn't have
		 * to be a fatal condition.
		 */
		build(vertexShaderObject->filename,   vertexShaderObject,
			  fragmentShaderObject->filename, fragmentShaderObject, programObject);

		/* Everything worked! Free our old resources and take up the new ones. */
		releaseGLStuff();
		this->vertexShaderObject = vertexShaderObject;
		this->fragmentShaderObject = fragmentShaderObject;
		this->programObject = programObject;
		return true;
	}
	catch (ShaderBuildException ex)
	{
		cerr << "ShaderProgram(\"" << vertexShaderObject->filename << "\", \""
			 << fragmentShaderObject->filename << "\") caught shrapnel" << endl;
		return false;
	}
}

void ShaderProgram::build(
	string vertexShaderFilename,
	VertexShaderObject*& vertexShaderObject,
	string fragmentShaderFilename,
	FragmentShaderObject*& fragmentShaderObject,
	GLuint& programObject)
{
	vertexShaderObject = new VertexShaderObject(vertexShaderFilename);
	fragmentShaderObject = new FragmentShaderObject(fragmentShaderFilename);

	/* Create OpenGL Shader Program Object */
	programObject = glCreateProgram();
	if (programObject == 0)
		throw ShaderBuildException("glCreateProgram() = 0");

	/* Link the program */
	glAttachShader(programObject, fragmentShaderObject->shaderObject);
	glAttachShader(programObject, vertexShaderObject->shaderObject);
	glLinkProgram(programObject);
	GLint iResult;
	glGetProgramiv(programObject, GL_LINK_STATUS, &iResult);
	if (iResult == GL_FALSE)
	{
		/* Link failed; request "info log" for compiler error details */
		/* Begin with fetching size of "info log" */
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &iResult);
		/* No info log! */
		if (iResult <= 1)
			throw ShaderBuildException("glLinkProgram() failed silently");
		/* Allocate memory for info log */
		char* infoLogBuffer = new char[iResult];
		/* Retrieve info log from GL */
		glGetProgramInfoLog(programObject, iResult, NULL, infoLogBuffer);
		/* Throw */
		string errorMessage(infoLogBuffer);
		delete infoLogBuffer;
		throw ShaderBuildException(errorMessage);
	}
}

void ShaderProgram::releaseGLStuff()
{
	if (programObject != 0)
	{
		glDeleteProgram(programObject);
		programObject = 0;
	}
	if (vertexShaderObject != NULL)
	{
		delete vertexShaderObject;
		vertexShaderObject = NULL;
	}
	if (fragmentShaderObject != NULL)
	{
		delete fragmentShaderObject;
		fragmentShaderObject = NULL;
	}
}

void ShaderProgram::use()
{
	glUseProgram(programObject);
}

GLint ShaderProgram::getAttribLocation(const char* name)
{
	return glGetAttribLocation(programObject, name);
}

GLint ShaderProgram::getUniformLocation(const char* name)
{
	return glGetUniformLocation(programObject, name);
}

ShaderBuildException::ShaderBuildException(string errorMessage) :
	errorMessage(errorMessage)
{
}

const char* ShaderBuildException::what() const throw ()
{
	return errorMessage.c_str();
}
