#include "Shader.h"

Shader::Shader(const std::string& name)
{
	m_IsLink = false;
	m_IsComputeShader = false;

	m_ProgramID = glCreateProgram();
}
Shader::~Shader()
{
	glDeleteProgram(m_ProgramID);
}

Shader* Shader::AttachShader(BaseShader baseShader)
{
	glAttachShader(m_ProgramID, baseShader.GetID());
	m_Shaders.push_back(baseShader.GetID());

	return this;
}
void Shader::LinkProgram()
{
	glLinkProgram(m_ProgramID);

	if (CheckCompileError(m_ProgramID, "PROGRAM", ""))
	{
		m_IsLink = true;
		while (!m_Shaders.empty()) 
		{
			glDeleteShader(m_Shaders.back());
			m_Shaders.pop_back();
		}
	}
	else 
	{
		std::cerr << "ERROR WHILE LINKING TO " << m_Name << " PROGRAM" << "\n";
	}
}

void Shader::Activate() const
{
	if (!m_IsLink)
	{
		std::cerr << "ERROR: PROGRAM " << m_Name << " IS NOT LINK" << "\n";
	}

	glUseProgram(m_ProgramID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}
void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(m_ProgramID, name.c_str()), value);
}
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, &value[0]);
}
void Shader::SetMat4(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_ProgramID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
void Shader::SetSampler2D(const std::string& name, unsigned int texture, int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	this->SetInt(name, id);
}
void Shader::SetSampler3D(const std::string& name, unsigned int texture, int id) const 
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_3D, texture);
	this->SetInt(name, id);
}

unsigned int Shader::GetProgramID() const
{
	return m_ProgramID;
}