#include "FontManager.h"

namespace MotionByte
{
	FontManager::FontManager()
	{
		FT_Init_FreeType(&ft);
		loadFont("G:/Dev/MotionByte_framework/Resource/Roboto-Regular.ttf");
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
	}

	FontManager& MotionByte::FontManager::instance()
	{
		static FontManager instance;
		return instance;
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
		FT_Set_Pixel_Sizes(face, 0, 96.0);
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
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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

	void MotionByte::FontManager::RenderText(std::string text, float x, float y, float scale, Color color)
	{
		glEnableVertexArrayAttrib(vao, 3);
		glEnableVertexAttribArray(3);
		glUniform4f(6, color.getRed(), color.getGreen(), color.getBlue(),color.getAlpha());

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