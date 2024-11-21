#include "FontManager.h"
#include <cstdlib>
#include "Roboto-Regular.h"
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
		mProgram = ProgramManager::createCompiledProgram(textShaderSource, textFragmentShaderSource);
		glEnable(GL_MULTISAMPLE);
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
	
	std::shared_ptr<Font> FontManager::createFont()
    {
		std::shared_ptr<Font> font = std::make_shared<Font>();
		for (double i = FONT_MIN_SIZE; i< FONT_MAX_SIZE; i+= FONT_SIZE_STEP)
		{
			int index = font->getIndexForSize(i);
			glCreateVertexArrays(1, &font->vaoInVariousSize[index]);
			glBindVertexArray(font->vaoInVariousSize[index]);

			glCreateBuffers(1, &font->bufferInVariousSize[index]);

			glNamedBufferStorage(font->bufferInVariousSize[index], sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_STORAGE_BIT);
			glVertexArrayVertexBuffer(font->vaoInVariousSize[index], 3, font->bufferInVariousSize[index], 0, sizeof(GLfloat) * 4);

			// Update the vertex attribute index in the following lines
			glVertexArrayAttribFormat(font->vaoInVariousSize[index], 3, 4, GL_FLOAT, GL_FALSE, 0);
			glVertexArrayAttribBinding(font->vaoInVariousSize[index], 3, 3);
			glEnableVertexArrayAttrib(font->vaoInVariousSize[index], 3);
			glEnableVertexAttribArray(3);
		}
		return font;

    }
	std::shared_ptr<Font> FontManager::createFont(std::string fontPath)
    {
		std::shared_ptr<Font> font = createFont();
		loadFont(*font, fontPath);
        return font;
    }
    std::shared_ptr<Font> FontManager::createFont(unsigned char data[], unsigned int size)
    {
		std::shared_ptr<Font> font = createFont();
		loadFont(*font, data, size);
        return font;
    }

    std::shared_ptr<Font> FontManager::createDefaultFont()
    {
        return createFont(_Roboto_Regular_ttf,sizeof(_Roboto_Regular_ttf));
    }

    void FontManager::initAfterLoad(Font &font)
    {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		for (double i = FONT_MIN_SIZE; i<=FONT_MAX_SIZE;i+=FONT_SIZE_STEP)
		{
			int index = font.getIndexForSize(i);
			for (GLubyte c = 0; c < 128; c++) {
				FT_Load_Char(font.faceInVariousSize[index], c, FT_LOAD_RENDER);

				GLuint texture;
				glCreateTextures(GL_TEXTURE_2D, 1, &texture);
				glTextureStorage2D(texture, 1, GL_R8, font.faceInVariousSize[index]->glyph->bitmap.width, font.faceInVariousSize[index]->glyph->bitmap.rows);
				glTextureSubImage2D(texture, 0, 0, 0, font.faceInVariousSize[index]->glyph->bitmap.width, font.faceInVariousSize[index]->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, font.faceInVariousSize[index]->glyph->bitmap.buffer);

				glBindTexture(GL_TEXTURE_2D, texture);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);

				Character character = {
				texture,
				glm::ivec2(font.faceInVariousSize[index]->glyph->bitmap.width, font.faceInVariousSize[index]->glyph->bitmap.rows),
				glm::ivec2(font.faceInVariousSize[index]->glyph->bitmap_left, font.faceInVariousSize[index]->glyph->bitmap_top),
				font.faceInVariousSize[index]->glyph->advance.x
				};
				font.charactersInVariousSize[index].insert(std::pair<GLchar, Character>(c, character));
			}
			FT_Done_Face(font.faceInVariousSize[index]);
		}
		
	}
	void FontManager::loadFont(Font &font, std::string fontPath)
	{
		for (double i = FONT_MIN_SIZE; i<FONT_MAX_SIZE;i+=FONT_SIZE_STEP)
		{
			int index = font.getIndexForSize(i);
			if (FT_New_Face(ft, fontPath.c_str(), 0, &font.faceInVariousSize[index]))
			{
				fprintf(stderr, "Error opening font file\n");
				FT_Done_FreeType(ft);  // Cleanup FreeType library
				return;
			}
			FT_Set_Pixel_Sizes(font.faceInVariousSize[index], 0, i);
		}
		initAfterLoad(font);
	}
	void FontManager::loadFont(Font &font, unsigned char data[], unsigned int size)
	{
		for (double i = FONT_MIN_SIZE; i<=FONT_MAX_SIZE;i+=FONT_SIZE_STEP)
		{
			int index = font.getIndexForSize(i);
			if (FT_New_Memory_Face(ft, data, size, 0, &font.faceInVariousSize[index]))
			{
				fprintf(stderr, "Error opening font file\n");
				FT_Done_FreeType(ft);  // Cleanup FreeType library
				return;
			}
			FT_Set_Pixel_Sizes(font.faceInVariousSize[index], 0, i);
		}
		initAfterLoad(font);
	}
    Font::Font()
    {
		bufferInVariousSize.resize((FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP+1);
		vaoInVariousSize.resize((FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP+1);
		charactersInVariousSize.resize((FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP+1);
		faceInVariousSize.resize((FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP+1);
    }
    int Font::getIndexForSize(double size)
    {
		if (size < FONT_MIN_SIZE)
		   	return 0;
		if (size > FONT_MAX_SIZE)
			return bufferInVariousSize.size() - 1;
        return (size - FONT_MIN_SIZE) / FONT_SIZE_STEP;
    }
    double Font::fromIndexToSize(int index)
    {
        return index * FONT_SIZE_STEP + FONT_MIN_SIZE;
    }

    Font::~Font()
    {
		// free all resources
		for (int i = 0; i < bufferInVariousSize.size(); i++)
		{
			glDeleteBuffers(1, &bufferInVariousSize[i]);
			glDeleteVertexArrays(1, &vaoInVariousSize[i]);
		}
    }

	void FontManager::RenderText(Color color, Font& font, std::string text, float size, Rectangle bound, Align align)
    {
		useThisProgram();
		double width_of_text = 0;
		double height_of_text = 0;
		int index = font.getIndexForSize(size);
		double abs_size = font.fromIndexToSize(index);

		double newScale = size / abs_size;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = font.charactersInVariousSize[index][*c];
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

		RenderText(color, font, text, x, y, size);
    }
    void FontManager::RenderText(Color color, Font& font, std::string text, float x, float y, float size)
    {
		int index = font.getIndexForSize(size);
		double abs_size = font.fromIndexToSize(index);

		double newScale = size / abs_size;
		glEnableVertexArrayAttrib(font.vaoInVariousSize[index], 3);
		glEnableVertexAttribArray(3);
		glUniform4f(6, color.getRed(), color.getGreen(), color.getBlue(),color.getAlpha());
		y = mHeight - y;
		double scale = size / abs_size;
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++) {
			Character ch = font.charactersInVariousSize[index][*c];
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

			glNamedBufferSubData(font.bufferInVariousSize[index], 0, sizeof(GLfloat) * 6 * 4, vertices);
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (ch.Advance >> 6) * scale;
		}
    }
}