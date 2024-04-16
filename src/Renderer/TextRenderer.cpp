#include "TextRenderer.h"

TextRenderer* TextRenderer::m_Instance = nullptr;

TextRenderer::TextRenderer()
    : m_TextShader("TextShader")
{
    m_TextShader.AttachShader(BaseShader("Shader/Text.vert"));
    m_TextShader.AttachShader(BaseShader("Shader/Text.frag"));
    m_TextShader.LinkProgram();

    CreateBuffer();
}

void TextRenderer::Init()
{
    // Initialize Library
    if (FT_Init_FreeType(&m_Library))
    {
        std::cerr << "[ERROR::FREETYPE] Could not init FreeType Library\n";
        return;
    }

    if (FT_New_Face(m_Library, "Fonts/BAUHS93.ttf", 0, &m_Face))
    {
        std::cerr << "[ERROR::FREETYPE] Failed to load font\n";
        return;
    }

    FT_Set_Pixel_Sizes(m_Face, 0, BASE_FONT_SIZE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 255; c++)
    {
        // Load Character Glyph
        if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
        {
            std::cerr << "[ERROR::FREETYPE] Failed to load Glyph\n";
            return;
        }
        // Generate Texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_Face->glyph->bitmap.buffer);

        // Set Texture Options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store Character for later use
        Character character = {
            texture,
            glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
            glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
            m_Face->glyph->advance.x
        };
        Characters[c] = character;
    }
}
void TextRenderer::Reset()
{
    for (int idx = textList.size() - 1; idx >= 0; idx--)
    {
        delete textList[idx];
    }
    textList.clear();
}
void TextRenderer::Clean()
{
    TextRenderer::Reset();

    FT_Done_Face(m_Face);
    FT_Done_FreeType(m_Library);
    delete m_Instance;
}

Text* TextRenderer::AddText(const std::string& text, glm::vec2 pos, float fontsize, const glm::vec3& color)
{
    textList.push_back(new Text(text, pos, fontsize, color));
    return textList.back();
}
void TextRenderer::RenderText(Window& window, Camera& camera)
{
    m_TextShader.Activate();
    m_TextShader.SetMat4("u_View", camera.GetViewMatrix());
    m_TextShader.SetMat4("u_Projection", camera.GetPerspective(window));
    //m_TextShader.SetMat4("u_Projection", glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f));
    m_TextShader.SetInt("u_Text", 0);
    m_VAO.Bind();

    for (Text*& text : textList)
    {
        if (!text->active)
            continue;

        m_TextShader.SetVec3("u_TextColor", text->color);
        glm::vec3 pos = text->position;
        float scale = text->fontsize / BASE_FONT_SIZE;

        if (!text->IsVerticalOffsetAssign())
            CalculateVerticalOffset(*text);
        float verticalOffset = text->verticalOffset;
        
        std::string::const_iterator c;
        for (c = text->text.begin(); c != text->text.end(); c++)
        {
            Character ch = Characters[*c];

            float xpos = pos.x + ch.Bearing.x * scale;
            float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // Render Glyph Texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            glActiveTexture(GL_TEXTURE0);
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::translate(glm::mat4(1.0f), glm::vec3(xpos + w / 2.0f, ypos + h / 2.0f - verticalOffset / 2.0f, pos.z));
            model *= glm::scale(glm::mat4(1.0f), glm::vec3(w, h, 1.0f));
            m_TextShader.SetMat4("u_Model", model);

            /*
            glm::vec4 p = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f) * camera.GetViewMatrix() * model * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
            std::cout << p.x << " " << p.y << " " << p.z << " " << p.w << "\n";
            */

            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Advance cursors for next glyph
            pos.x += (ch.Advance >> 6) * scale;
        }
    }
    m_VAO.UnBind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Private Function
void TextRenderer::CreateBuffer()
{
    float vertices[6][4] = {
        { -0.5f, 0.5f, 0.0f, 0.0f },
        { -0.5f, -0.5f, 0.0f, 1.0f },
        { 0.5f, -0.5f, 1.0f, 1.0f },
                      
        { -0.5f, 0.5f, 0.0f, 0.0f },
        { 0.5f, -0.5f, 1.0f, 1.0f },
        { 0.5f, 0.5f, 1.0f, 0.0f }
    };

    m_VAO.Bind();
    m_VBO.Bind();

    m_VBO.BufferData(sizeof(float) * 6 * 4, vertices, true);
    m_VAO.Attribute(m_VBO, 0, 4, GL_FLOAT, 4 * sizeof(float), 0);

    m_VBO.UnBind();
    m_VAO.UnBind();
}

void TextRenderer::CalculateVerticalOffset(Text& text)
{
    glm::vec2 pos = text.position;
    float scale = text.fontsize / BASE_FONT_SIZE;
    std::string::const_iterator c;
    for (c = text.text.begin(); c != text.text.end(); c++)
    {
        Character& ch = Characters[*c];

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        pos.x += (ch.Advance >> 6) * scale;
        text.verticalOffset = std::max(h - (ch.Size.y - ch.Bearing.y) * scale, text.verticalOffset);
    }
    text.SetIsVerticalOffset(true);
}