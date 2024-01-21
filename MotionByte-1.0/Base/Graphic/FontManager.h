#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include "Color.h"
#include "../Segment/Rectangle.h"
#include "Align.h"
#include "ProgramManager.h"
namespace MotionByte
{
	
	class FontManager
	{
	private:
		GLuint mProgram;
		float mWidth, mHeight;
		const float FONT_RENDER_SIZE = 500.0;
		struct Character {
			GLuint     TextureID;  // ID handle of the glyph texture
			glm::ivec2 Size;       // Size of glyph
			glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
			GLuint     Advance;    // Offset to advance to next glyph
		};
		GLuint buffer;
		GLuint vao;
		std::map<GLchar, Character> Characters;
		FT_Library ft;
		FT_Face face;
		FontManager();
		void useThisProgram();
	public:


		static FontManager& instance();

		void onWindowSizeChanged(int width, int height);
		FT_Library& getFreeTypeLibrary();
		void loadFont(std::string fontPath);
		void update();
		void RenderText(Color color, std::string text, float size, Rectangle bound, Align align);
		void RenderText(std::string text, float x, float y, float size,
			Color color);
	};
}