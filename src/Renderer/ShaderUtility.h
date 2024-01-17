#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>

struct ShaderType
{
	unsigned int type;
	std::string name;

	ShaderType()
		: type(-1), name("")
	{
	}
	ShaderType(unsigned int type, const std::string& name)
		: type(type), name(name)
	{
	}
};

bool CheckCompileError(unsigned int shaderID, const std::string& type, const std::string& shaderName);
std::string GetShaderName(const char* shaderPath);
ShaderType GetShaderType(const char* path);