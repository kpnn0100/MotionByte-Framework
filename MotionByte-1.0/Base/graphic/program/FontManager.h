#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <vector>
#include "util/geometry/Color.h"
#include "util/geometry/Rectangle.h"
#include "util/geometry/Align.h"
#include "ProgramManager.h"
namespace MotionByte
{
	struct Character {
		std::vector<unsigned char> BitmapData; // Store bitmap data in RAM
		GLuint     Width;      // Width of glyph bitmap
		GLuint     Height;     // Height of glyph bitmap
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};
	class Font
    {
		friend class FontManager;
	private:
		std::vector<GLuint> bufferInVariousSize;
		std::vector<GLuint> vaoInVariousSize;
		std::vector<std::map<GLchar, Character>> charactersInVariousSize;
		std::vector<FT_Face> faceInVariousSize;
		std::vector<FT_Library> ft;
		int getIndexForSize(double size);
		double fromIndexToSize(int index);
	public:
		Font();
		~Font();
    };
	class FontManager
	{
	private:
		GLuint mProgram;
		GLuint mTextureID; // Single texture used for all characters
		float mWidth, mHeight;
		const float FONT_RENDER_SIZE = 128.0;
		
		
		FontManager();
		void loadFont(Font &font,std::string fontPath);
		void loadFont(Font &font, unsigned char data[], unsigned int size);
		void initAfterLoad(Font &font, double size);
		void useThisProgram();
	public:
		static FontManager& instance();
		void onWindowSizeChanged(int width, int height);
		std::shared_ptr<Font> createFont();
		std::shared_ptr<Font> createFont(std::string fontPath);
		std::shared_ptr<Font> createFont(unsigned char data[], unsigned int size);
		std::shared_ptr<Font> createDefaultFont();
		void RenderText(Color color, Font& font, std::string text, float size, Rectangle bound, Align align);
		void RenderText(Color color, Font& font, std::string text, float x, float y, float size);
	};
}