/* vim: ts=4 sts=4 sw=4 noexpandtab: */
#ifndef SHADER_H_IEJWF20FH28GF982
#define SHADER_H_IEJWF20FH28GF982
#include <platecommon.h>
#include <string>
#include <exception>

class ShaderBuildException : public std::exception
{
	public:
	std::string errorMessage;
	ShaderBuildException(std::string errorMessage);
	virtual const char* what() const throw () override;
};

template <GLenum SHADER_TYPE>
class ShaderObject
{
	public:
	std::string filename;
	GLuint shaderObject;
	ShaderObject(std::string filename);
	~ShaderObject();
	private:
	void releaseGLStuff();
};
typedef ShaderObject<GL_VERTEX_SHADER> VertexShaderObject;
typedef ShaderObject<GL_FRAGMENT_SHADER> FragmentShaderObject;

class ShaderProgram
{
	public:
	VertexShaderObject* vertexShaderObject;
	FragmentShaderObject* fragmentShaderObject;
	GLuint programObject;
	ShaderProgram(std::string vertexShaderFilename, std::string frag);
	~ShaderProgram();
	/* Returns false if an error occurred, but this instance will continue to
	 * function using the original shader program, and an error message will
	 * be written to stderr.
	 */
	bool rebuild();
	void use();
	GLint getAttribLocation(const char* name);
	GLint getUniformLocation(const char* name);
	private:
	void build(
		std::string vertexShaderFilename,
		VertexShaderObject*& vertexShaderObject,
		std::string fragmentShaderFilename,
		FragmentShaderObject*& fragmentShaderObject,
		GLuint& programObject);
	void releaseGLStuff();
};

#endif/*SHADER_H_IEJWF20FH28GF982*/
