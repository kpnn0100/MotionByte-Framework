#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "util/geometry/Color.h"
#include "util/geometry/Rectangle.h"
#include "util/geometry/Align.h"
#include "ProgramManager.h"
#define FONT_MIN_SIZE 12
#define FONT_MAX_SIZE 12
#define FONT_SIZE_STEP 1
namespace MotionByte
{
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
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
		float mWidth, mHeight;
		const float FONT_RENDER_SIZE = 128.0;
		FT_Library ft;
		
		FontManager();
		void loadFont(Font &font,std::string fontPath);
		void loadFont(Font &font, unsigned char data[], unsigned int size);
		void initAfterLoad(Font &font);
		void useThisProgram();
	public:


		static FontManager& instance();
		void onWindowSizeChanged(int width, int height);
		FT_Library& getFreeTypeLibrary();
		std::shared_ptr<Font> createFont();
		std::shared_ptr<Font> createFont(std::string fontPath);
		std::shared_ptr<Font> createFont(unsigned char data[], unsigned int size);
		std::shared_ptr<Font> createDefaultFont();
		void RenderText(Color color, Font& font, std::string text, float size, Rectangle bound, Align align);
		void RenderText(Color color, Font& font, std::string text, float x, float y, float size);
	};
}