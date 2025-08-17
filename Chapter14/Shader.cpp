#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"

constexpr int BUFFER_SIZE = 512;

Shader::Shader()
	: mShaderProgram(0)
	, mVertexShader(0)
	, mFragShader(0)
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
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniformMatrix4fv(loc, 1, GL_TRUE, matrix.GetAsFloatPtr());
}

void Shader::SetMatrixUniforms(const char* name, const Matrix4* const pMatrices, const unsigned int count)
{
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniformMatrix4fv(loc, count, GL_TRUE, pMatrices->GetAsFloatPtr());
}

void Shader::SetVectorUniform(const char* name, const Vector3& vector)
{
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform3fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetVector2Uniform(const char* name, const Vector2& vector)
{
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform2fv(loc, 1, vector.GetAsFloatPtr());
}

void Shader::SetFloatUniform(const char* name, const float value)
{
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform1f(loc, value);
}

void Shader::SetIntUniform(const char* name, const int value)
{
	const GLuint loc = glGetUniformLocation(mShaderProgram, name);

	glUniform1i(loc, value);
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
			char buffer[BUFFER_SIZE] = { '\0', };
			glGetShaderInfoLog(outShader, BUFFER_SIZE, nullptr, buffer);

			SDL_Log("Failed to compile shader %s: %s", fileName.c_str(), buffer);

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

bool Shader::isCompiled(const GLuint shader)
{
	GLint status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
	{
		char buffer[BUFFER_SIZE] = { '\0', };

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
		char buffer[BUFFER_SIZE] = { '\0', };

		glGetProgramInfoLog(mShaderProgram, BUFFER_SIZE - 1, nullptr, buffer);
		SDL_Log("GLSL Link Status:\n%s", buffer);

		return false;
	}

	return true;
}
