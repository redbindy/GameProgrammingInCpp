#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <SDL3/SDL.h>

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

	void SetVectorUniform(const char* name, const Vector3& vector);

	void SetFloatUniform(const char* name, const float value);

private:
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;

private:
	bool compileShader(const std::string& fileName, GLenum shaderType, GLuint& outShader);
	bool isCompiled(GLuint shader);
	bool isValidProgram();
};