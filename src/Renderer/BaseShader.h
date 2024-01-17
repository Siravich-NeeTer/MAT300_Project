#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "ShaderUtility.h"

class BaseShader
{
	public:
		BaseShader(const char* shaderPath);
		~BaseShader();

		unsigned int GetID() const { return m_ShaderID; }
		ShaderType GetType() const { return m_ShaderType; }
		std::string GetPath() const { return m_ShaderPath; }
		std::string GetName() const { return GetShaderName(m_ShaderPath.c_str()); }

	private:
		std::string m_ShaderPath;

		unsigned int m_ShaderID;
		ShaderType m_ShaderType;

		std::string LoadShaderCodeFromFile(const char* shaderPath);
};
