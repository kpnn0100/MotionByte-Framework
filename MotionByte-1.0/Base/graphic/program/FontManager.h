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
#define FONT_MIN_SIZE 1     // Higher resolution minimum size
#define FONT_MAX_SIZE 200     // Higher resolution maximum size
#define FONT_SIZE_STEP 1    // Larger step for more resolution options
namespace MotionByte
{
	struct Character {
		std::vector<std::vector<Vertex>> ContourList; // List of contours for triangulation
		VertexList Vertices;        // Vertices for triangulation
		glm::ivec2 Size;                  // Size of glyph
		glm::ivec2 Bearing;               // Offset from baseline to left/top of glyph
		GLuint Advance;                   // Offset to advance to next glyph
		double RenderSize;             // Size for rendering
	};
	class Font
    {
		friend class FontManager;
	private:
		std::vector<std::map<uint8_t, Character>> characters;
		FT_Face face;
		FT_Library ft;
		double getNearestSize(double size) {
			double sizeIndex = (size - FONT_MIN_SIZE) / FONT_SIZE_STEP;
			int index = (int)round(sizeIndex);
			index = std::max(0, std::min(index, (FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP));
			return FONT_MIN_SIZE + index * FONT_SIZE_STEP;
		}
		int getIndexForSize(double size) {
			double sizeIndex = (size - FONT_MIN_SIZE) / FONT_SIZE_STEP;
			int index = (int)round(sizeIndex);
			index = std::max(0, std::min(index, (FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP));
			return index;
		}
		VertexList getTriangluationVertex(std::vector<std::vector<Vertex>> ContourList);
	public:
		Character& getCharacter(uint8_t c, double size) {
			return characters[getIndexForSize(size)][c];
		}
		double getWidth(std::string text, double size) {
			double width = 0.0;
			double scale = size / getNearestSize(size);
			for (char c : text) {
				Character& character = getCharacter(c, size);
				width += (character.Advance >> 6) * scale; // Scale by render size
			}
			return width;
		}
		double getHeight(std::string text, double size) {
			double height = 0.0;
			double scale = size / getNearestSize(size);
			for (char c : text) {
				Character& character = getCharacter(c, size);
				if (character.Bearing.y * scale > height) {
					height = character.Bearing.y * scale; // Scale by render size
				}
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
	};
}