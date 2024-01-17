#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <list>

#include "BaseShader.h"

class Shader
{
	public:
		Shader(const std::string& name);
		~Shader();

		Shader* AttachShader(BaseShader baseShader);
		void LinkProgram();

		void Activate() const;

		void SetBool(const std::string& name, bool value) const;
		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
		void SetVec2(const std::string& name, const glm::vec2& value) const;
		void SetVec3(const std::string& name, const glm::vec3& value) const;
		void SetVec4(const std::string& name, const glm::vec4& value) const;
		void SetMat4(const std::string& name, const glm::mat4& value) const;
		void SetSampler2D(const std::string& name, unsigned int texture, int id) const;
		void SetSampler3D(const std::string& name, unsigned int texture, int id) const;

		GLuint GetProgramID() const;

	private:
		GLuint m_ProgramID;
		std::list<unsigned int> m_Shaders;
		std::string m_Name;

		bool m_IsLink;
		bool m_IsComputeShader;
};