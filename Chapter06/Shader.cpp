#include "Shader.h"
#include "Mesh.h"

Shader::Shader()
	: mVertexShader(0)
	, mFragShader(0)
	, mShaderProgram(0)
{

}

bool Shader::Load(const std::string& vertName, const std::string& fragName)
{
	if (!compileShader(vertName, GL_VERTEX_SHADER, mVertexShader)
		|| !compileShader(fragName, GL_FRAGMENT_SHADER, mFragShader))
	{
		return false;
	}

	mShaderProgram = glCreateProgram();
	glAttachShader(mShaderProgram, mVertexShader);
	glAttachShader(mShaderProgram, mFragShader);
	glLinkProgram(mShaderProgram);

	return isValidProgram();
}

void Shader::Unload()
{
	glDeleteProgram(mShaderProgram);
	glDeleteShader(mFragShader);
	glDeleteShader(mVertexShader);
}

void Shader::SetActive()
{
	glUseProgram(mShaderProgram);
}

void Shader::SetMatrixUniform(const char* name, const Matrix4& matrix)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniformMatrix4fv(
		loc,
		1,
		GL_TRUE,
		matrix.GetAsFloatPtr()
	);
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, const float value)
{
	GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform1f(loc, value);
}

bool Shader::compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader)
{
	std::ifstream shaderFile(fileName);
	if (shaderFile.is_open())
	{
		std::stringstream sstream;
		sstream << shaderFile.rdbuf();

		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(shaderType);

		glShaderSource(outShader, 1, &contentsChar, nullptr);
		glCompileShader(outShader);

		if (!isCompiled(outShader))
		{
			SDL_Log("Failed to compile shader %s", fileName.c_str());

			return false;
		}
	}
	else
	{
		SDL_Log("Shader file not found: %s", fileName.c_str());

		return false;
	}

	return true;
}

bool Shader::isCompiled(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		constexpr int BUFFER_SIZE = 512;

		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		glGetShaderInfoLog(shader, BUFFER_SIZE - 1, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);

		return false;
	}

	return true;
}

bool Shader::isValidProgram()
{
	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
	{
		constexpr int BUFFER_SIZE = 512;

		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		glGetProgramInfoLog(mShaderProgram, BUFFER_SIZE - 1, nullptr, buffer);
		SDL_Log("GLSL Compile Failed:\n%s", buffer);

		return false;
	}

	return true;
}
