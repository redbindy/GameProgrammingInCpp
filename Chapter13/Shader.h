#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <SDL3/SDL_log.h>
#include <GL/glew.h>

#include "Math.h"

class Shader final
{
public:
	Shader();
	virtual ~Shader() = default;

	bool Load(const std::string& vertName, const std::string& fragName);
	void Unload();

	void SetActive();

	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	void SetMatrixUniforms(const char* name, const Matrix4* const pMatrices, const unsigned int count);

	void SetVectorUniform(const char* name, const Vector3& vector);
	void SetVector2Uniform(const char* name, const Vector2& vector);

	void SetFloatUniform(const char* name, const float value);
	void SetIntUniform(const char* name, const int value);

private:
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool isCompiled(const GLuint shader);
	bool isValidProgram();
};