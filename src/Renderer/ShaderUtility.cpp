#include "ShaderUtility.h"

bool CheckCompileError(unsigned int shaderID, const std::string& type, const std::string& shaderName)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
			std::cerr << "ERROR: SHADER " << shaderName << " " << type << "::COMPILATION_ERROR\n";
			std::cerr << "-------------------------------------------------\n";
			std::cerr << infoLog << '\n';
		}
	}
	else
	{
		glGetProgramiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
			std::cerr << "ERROR::PROGRAM_LINKING_ERROR::" << type << "\n";
			std::cerr << "-------------------------------------------------\n";
			std::cerr << infoLog << '\n';
		}
	}

	return success;
}
std::string GetShaderName(const char* shaderPath)
{
	std::string fileName = shaderPath;
	int lastSlashIndex = fileName.find_last_of("/");
	if (std::string::npos != lastSlashIndex)
	{
		fileName.erase(0, lastSlashIndex + 1);
	}
	return fileName;
}
ShaderType GetShaderType(const char* path)
{
	std::string type = GetShaderName(path);
	int fileTypeIndex = type.find_last_of(".");
	if (std::string::npos != fileTypeIndex)
	{
		type.erase(0, fileTypeIndex + 1);
	}

	// Vertex Shader
	if (type == "vert")
	{
		return ShaderType(GL_VERTEX_SHADER, "VERTEX");
	}
	// Fragment Shader
	else if (type == "frag")
	{
		return ShaderType(GL_FRAGMENT_SHADER, "FRAGMENT");
	}
	// Tessellation Evaluation Shader
	else if (type == "tes")
	{
		return ShaderType(GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
	}
	// Tessellation Control Shader
	else if (type == "tcs")
	{
		return ShaderType(GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
	}
	// Geometry Shader
	else if (type == "geom")
	{
		return ShaderType(GL_GEOMETRY_SHADER, "GEOMETRY");
	}
	// Compute Shader
	else if (type == "comp")
	{
		return ShaderType(GL_COMPUTE_SHADER, "COMPUTE");
	}
}