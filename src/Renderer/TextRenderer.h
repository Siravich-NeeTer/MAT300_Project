#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "Shader.h"

#include "../Camera.h"

#define BASE_FONT_SIZE 96

struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};
class Text
{
	public:
		std::string text;
		glm::vec2 position;
		float fontsize;
		glm::vec3 color;
		bool active = true;

		float verticalOffset;
	
		Text(const std::string& _text, const glm::vec2& _position, const float& _fontsize, const glm::vec3& _color, const float& _verticalOffset = 0)
			: text(_text), position(_position), fontsize(_fontsize), color(_color), 
			verticalOffset(_verticalOffset), isVerticalOffsetAssign(false)
		{
		}

		void SetIsVerticalOffset(const bool& active) { isVerticalOffsetAssign = active; }
		bool IsVerticalOffsetAssign() const { return isVerticalOffsetAssign; }

	private:
		// Initalize as a false
		bool isVerticalOffsetAssign;
};

class TextRenderer
{
	public:

		TextRenderer();
		static TextRenderer* GetInstance() 
		{
			if (m_Instance == nullptr)
			{
				m_Instance = new TextRenderer();
				m_Instance->Init();
			}
			return m_Instance; 
		}

		void Init();
		void Reset();
		void Clean();

		Text* AddText(const std::string& text, glm::vec2 pos, float fontsize, const glm::vec3& color);
		void RenderText(Window& window, Camera& camera);

	private:
		// Declare TextRenderer as Singleton
		static TextRenderer* m_Instance;
		std::vector<Text*> textList;

		FT_Library m_Library;
		FT_Face m_Face;

		std::map<char, Character> Characters;

		VertexArrayObject m_VAO;
		VertexBufferObject m_VBO;
		Shader m_TextShader;
		void CreateBuffer();

		void CalculateVerticalOffset(Text& text);
};