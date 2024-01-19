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
namespace MotionByte
{
	class FontManager
	{
	private:
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
	public:
		static FontManager& instance();
		FT_Library& getFreeTypeLibrary();
		void loadFont(std::string fontPath);
		void update();
		void RenderText(std::string text, float x, float y, float scale,
			Color color);
	};
}