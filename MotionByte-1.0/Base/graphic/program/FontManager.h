#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
#include <util/geometry/Vertex.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include <map>
#include <vector>
#include "util/geometry/Color.h"
#include "util/geometry/Rectangle.h"
#include "util/geometry/Align.h"
#include "ProgramManager.h"
#define FONT_RENDER_SIZE 48 // Single reference render size
namespace MotionByte
{
	struct Character {
		std::vector<std::vector<Vertex>> ContourList; // List of contours for triangulation
		VertexList Vertices;        // Vertices for triangulation
		glm::ivec2 Size;                  // Size of glyph
		glm::ivec2 Bearing;               // Offset from baseline to left/top of glyph
		GLuint Advance;                   // Offset to advance to next glyph
		double RenderSize;             // Base size used when generating this glyph
	};
	class Font
    {
		friend class FontManager;
	private:
		std::map<uint8_t, Character> characters; // Single size character set
		FT_Face face{};
		FT_Library ft{};
	public:
		Character& getCharacter(uint8_t c) { return characters[c]; }
		double getWidth(std::string text, double size) {
			if (characters.empty()) return 0.0; 
			double base = characters.begin()->second.RenderSize; if (base == 0) base = FONT_RENDER_SIZE;
			double scale = size / base;
			double width = 0.0;
			for (char c : text) {
				Character &character = characters[c];
				width += (character.Advance >> 6) * scale;
			}
			return width;
		}
		double getHeight(std::string text, double size) {
			if (characters.empty()) return 0.0;
			double base = characters.begin()->second.RenderSize; if (base == 0) base = FONT_RENDER_SIZE;
			double scale = size / base;
			double height = 0.0;
			for (char c : text) {
				Character &character = characters[c];
				if (character.Bearing.y * scale > height) height = character.Bearing.y * scale;
			}
			return height;
		}
		Font();
		~Font();
    };
	class FontManager
	{
	private:
		GLuint mProgram;
		GLuint mTextureID; // Single texture used for all characters
		float mWidth, mHeight;
		
		FontManager();
		void loadFont(Font &font, std::string fontPath);
		void loadFont(Font &font, unsigned char data[], unsigned int size);
		void initAfterLoad(Font &font);
	public:
		static FontManager& instance();
		std::shared_ptr<Font> createFont();
		std::shared_ptr<Font> createFont(std::string fontPath);
		std::shared_ptr<Font> createFont(unsigned char data[], unsigned int size);
		std::shared_ptr<Font> createDefaultFont();
		void RenderText(Color color, Font& font, std::string text, float size, Rectangle bound, Align align);
		void RenderText(Color color, Font& font, std::string text, float x, float y, float size);
		// Triangulate a list of contours (outer + holes). Orientation determines add/subtract.
		VertexList getTriangulation(std::vector<VertexList> & ContourList);
	};
}