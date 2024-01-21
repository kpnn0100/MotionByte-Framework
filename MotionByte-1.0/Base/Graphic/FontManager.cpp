#include "FontManager.h"
#include <cstdlib>
#include <windows.h> 
const char* textShaderSource = R"(
	#version 460 core
	layout (location = 3) in vec4 vertex; // <vec2 pos, vec2 tex>
	layout (location = 4) uniform mat4 projection;

	out vec2 TexCoords;

	void main()
	{
		gl_Position = projection * vec4(vertex.x, vertex.y, 0.0, 1.0);
		TexCoords = vertex.zw;
	}  
)";

const char* textFragmentShaderSource = R"(
	#version 460 core
	in vec2 TexCoords;
	out vec4 color;

	layout (binding = 0) uniform sampler2D text;
	layout (location = 6) uniform vec4 textColor;

	void main()
	{    
		color = vec4(textColor.rgb,textColor.a * texture(text, TexCoords).r);
        
	} 
)";
namespace MotionByte
{
	FontManager::FontManager()
	{
		FT_Init_FreeType(&ft);
		char buffer2[MAX_PATH];
		GetModuleFileName(NULL, buffer2, MAX_PATH);

		std::cout << "Executable path: " << buffer2 << std::endl;
		
		mProgram = ProgramManager::createCompiledProgram(textShaderSource, textFragmentShaderSource);
		glEnable(GL_MULTISAMPLE);
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glCreateBuffers(1, &buffer);

		glNamedBufferStorage(buffer, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
		glVertexArrayVertexBuffer(vao, 3, buffer, 0, sizeof(GLfloat) * 4);

		// Update the vertex attribute index in the following lines
		glVertexArrayAttribFormat(vao, 3, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 3, 3);
		glEnableVertexArrayAttrib(vao, 3);
		glEnableVertexAttribArray(3);
		loadFont("../../../Resource/Roboto-Regular.ttf");
	}

	void FontManager::useThisProgram()
	{
		glUseProgram(mProgram);
		glm::mat4 projection = glm::ortho(0.0f, mWidth, 0.0f, mHeight);
		GLint projectionLocation = glGetUniformLocation(mProgram, "projection");
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
	}

	FontManager& MotionByte::FontManager::instance()
	{
		static FontManager instance;
		return instance;
	}

	void FontManager::onWindowSizeChanged(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	FT_Library& MotionByte::FontManager::getFreeTypeLibrary()
	{
		return ft;
	}

	void MotionByte::FontManager::loadFont(std::string fontPath)
	{
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
		{
			fprintf(stderr, "Error opening font file\n");
			FT_Done_FreeType(ft);  // Cleanup FreeType library
			return;
		}
		FT_Set_Pixel_Sizes(face, 0, FONT_RENDER_SIZE);
		update();
	}

	void MotionByte::FontManager::update()
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++) {
			FT_Load_Char(face, c, FT_LOAD_RENDER);

			GLuint texture;
			glCreateTextures(GL_TEXTURE_2D, 1, &texture);
			glTextureStorage2D(texture, 1, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows);
			glTextureSubImage2D(texture, 0, 0, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			glBindTexture(GL_TEXTURE_2D, texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		/*FT_Done_Face(face);
		FT_Done_FreeType(ft);*/
	}

	void FontManager::RenderText(Color color, std::string text, float scale, Rectangle bound, Align align)
	{
		useThisProgram();
		double width_of_text = 0;
		double height_of_text = 0;
		double newScale = scale / FONT_RENDER_SIZE;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = Characters[*c];
			if (ch.Bearing.y* newScale > height_of_text)
			{
				height_of_text = ch.Bearing.y * newScale;
			}
			if (c != text.end() - 1)
			{
				width_of_text += (ch.Advance>>6) * newScale;
			}
			else
			{
				width_of_text += (ch.Bearing.x + ch.Size.x) * newScale;
			}
		}
		float x = 0;
		float y = 0;
		switch (align.getHorizontal())
		{
			case Align::Horizontal::Left:
			{
				x = bound.getCorner(bound.TopLeft).getX();
				break;
			}
			case Align::Horizontal::Middle:
			{
				x = bound.getCenter().getX() - width_of_text / 2.0;
				break;
			}
			case Align::Horizontal::Right:
			{
				x = bound.getCorner(bound.TopRight).getX() - width_of_text;
				break;
			}
		}
		switch (align.getVertical())
		{
			case Align::Vertical::Top:
			{
				y = bound.getCorner(bound.TopLeft).getY() + height_of_text;
				break;
			}
			case Align::Vertical::Center:
			{
				y = bound.getCenter().getY() + height_of_text / 2.0;
				break;
			}
			case Align::Vertical::Bottom:
			{
				y = bound.getCorner(bound.BottomLeft).getY();
				break;
			}
		}

		RenderText(text, x,y, scale, color);
	}

	void MotionByte::FontManager::RenderText(std::string text, float x, float y, float scale, Color color)
	{
		glEnableVertexArrayAttrib(vao, 3);
		glEnableVertexAttribArray(3);
		glUniform4f(6, color.getRed(), color.getGreen(), color.getBlue(),color.getAlpha());
		y = mHeight - y;
		scale /= FONT_RENDER_SIZE;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = Characters[*c];
			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// Update VBO for each character
			GLfloat vertices[6 * 4] = {
				 xpos,     ypos + h,   0.0f, 0.0f ,
				 xpos,     ypos,       0.0f, 1.0f ,
				 xpos + w, ypos,       1.0f, 1.0f ,

				 xpos,     ypos + h,   0.0f, 0.0f ,
				 xpos + w, ypos,       1.0f, 1.0f ,
				 xpos + w, ypos + h,   1.0f, 0.0f
			};

			glNamedBufferSubData(buffer, 0, sizeof(GLfloat) * 6 * 4, vertices);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (ch.Advance >> 6) * scale;

		}
	}
}