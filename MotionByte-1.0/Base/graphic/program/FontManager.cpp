#include "FontManager.h"
#include <cstdlib>
#include "Roboto-Regular.h"
#include "graphic/program/ShapeManager.h"
#define FONT_MIN_SIZE 2     // Higher resolution minimum size
#define FONT_MAX_SIZE 200     // Higher resolution maximum size
#define FONT_SIZE_STEP 2    // Larger step for more resolution options
#define FONT_RENDER_SIZE 48 // Reference size for vector outlines
#include <vector>
#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <cmath>



namespace MotionByte
{
	using Contour = std::vector<Vertex>;
	using OutlineContours = std::vector<Contour>;

	struct OutlineDecomposeState {
		OutlineContours* contours;
		Contour* currentContour;
		int resolution;
	};

	static int move_to_callback(const FT_Vector* to, void* user) {
		auto* state = reinterpret_cast<OutlineDecomposeState*>(user);
		state->contours->emplace_back(); // Start new contour
		state->currentContour = &state->contours->back();
		state->currentContour->push_back({ to->x / 64.0f, to->y / 64.0f });
		return 0;
	}

	static int line_to_callback(const FT_Vector* to, void* user) {
		auto* state = reinterpret_cast<OutlineDecomposeState*>(user);
		state->currentContour->push_back({ to->x / 64.0f, to->y / 64.0f });
		return 0;
	}

	static int conic_to_callback(const FT_Vector* control, const FT_Vector* to, void* user) {
		auto* state = reinterpret_cast<OutlineDecomposeState*>(user);
		Vertex from = state->currentContour->back();
		for (int i = 1; i <= state->resolution; ++i) {
			float t = static_cast<float>(i) / state->resolution;
			float mt = 1.0f - t;

			float x = mt * mt * from.x +
					2 * mt * t * (control->x / 64.0f) +
					t * t * (to->x / 64.0f);
			float y = mt * mt * from.y +
					2 * mt * t * (control->y / 64.0f) +
					t * t * (to->y / 64.0f);
			state->currentContour->push_back({ x, y });
		}
		return 0;
	}

	static int cubic_to_callback(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user) {
		auto* state = reinterpret_cast<OutlineDecomposeState*>(user);
		Vertex from = state->currentContour->back();
		for (int i = 1; i <= state->resolution; ++i) {
			float t = static_cast<float>(i) / state->resolution;
			float mt = 1.0f - t;

			float x = mt * mt * mt * from.x +
					3 * mt * mt * t * (control1->x / 64.0f) +
					3 * mt * t * t * (control2->x / 64.0f) +
					t * t * t * (to->x / 64.0f);

			float y = mt * mt * mt * from.y +
					3 * mt * mt * t * (control1->y / 64.0f) +
					3 * mt * t * t * (control2->y / 64.0f) +
					t * t * t * (to->y / 64.0f);

			state->currentContour->push_back({ x, y });
		}
		return 0;
	}

	OutlineContours decomposeOutlineToContours(FT_Outline& outline, int resolution) {
		OutlineContours contours;
		OutlineDecomposeState state;
		state.contours = &contours;
		state.currentContour = nullptr;
		state.resolution = resolution;

		FT_Outline_Funcs funcs;
		funcs.move_to = move_to_callback;
		funcs.line_to = line_to_callback;
		funcs.conic_to = conic_to_callback;
		funcs.cubic_to = cubic_to_callback;
		funcs.shift = 0;
		funcs.delta = 0;

		FT_Outline_Decompose(&outline, &funcs, &state);
		return contours;
	}
    FontManager::FontManager()
    {

    }

    FontManager& MotionByte::FontManager::instance()
    {
        static FontManager instance;
        return instance;
    }
    
    VertexList MotionByte::FontManager::getTriangluationVertexList(std::vector<std::vector<Vertex>> ContourList)
    {
        
    }

    std::shared_ptr<Font> FontManager::createFont()
    {
        std::shared_ptr<Font> font = std::make_shared<Font>();
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
		for (int size = 0; size < (FONT_MAX_SIZE - FONT_MIN_SIZE)/FONT_SIZE_STEP; ++size) {
			double renderSize = FONT_MIN_SIZE + size*FONT_SIZE_STEP;
			FT_Set_Pixel_Sizes(font.face, 0, renderSize);
			for (GLubyte c = 0; c < 128; c++) {
				// Load the glyph with FT_LOAD_NO_BITMAP to get vector outlines
				if (FT_Load_Char(font.face, c, FT_LOAD_DEFAULT))
					continue;
				
				// Store vector outline data
				Character character;
				character.Size = glm::ivec2(font.face->glyph->metrics.width >> 6, 
							font.face->glyph->metrics.height >> 6);
				character.Bearing = glm::ivec2(font.face->glyph->bitmap_left, 
							font.face->glyph->bitmap_top);
				character.Advance = font.face->glyph->advance.x;
				
				// Extract outline points and contours
				FT_Outline& outline = font.face->glyph->outline;
				
				// Process each contour in the outline
				int startPoint = 0;
				character.ContourList = decomposeOutlineToContours(outline, size);
                character.Vertices = getTriangluationVertex(character.ContourList);
				character.RenderSize = renderSize;
				font.characters[size].insert(std::pair<GLchar, Character>(c, character));
			}
		}
        
    }

    void FontManager::loadFont(Font &font, std::string fontPath)
    {
        FT_Init_FreeType(&font.ft);
        if (FT_New_Face(font.ft, fontPath.c_str(), 0, &font.face))
        {
            fprintf(stderr, "Error opening font file\n");
            FT_Done_FreeType(font.ft);  // Cleanup FreeType library
            return;
        }
        initAfterLoad(font);
    }

    void FontManager::loadFont(Font &font, unsigned char data[], unsigned int size)
    {
        FT_Init_FreeType(&font.ft);
        if (FT_New_Memory_Face(font.ft, data, size, 0, &font.face))
        {
            fprintf(stderr, "Error opening font file\n");
            FT_Done_FreeType(font.ft);  // Cleanup FreeType library
            return;
        }
        initAfterLoad(font);
    }

    Font::Font()
    {
        // Empty constructor
		characters.resize((FONT_MAX_SIZE - FONT_MIN_SIZE) / FONT_SIZE_STEP + 1);
    }

    Font::~Font()
    {
        // Clean up FreeType resources
        if (face) {
            FT_Done_Face(face);
        }
        if (ft) {
            FT_Done_FreeType(ft);
        }
    }

    void FontManager::RenderText(Color color, Font& font, std::string text, float size, Rectangle bound, Align align)
    {
        double width_of_text = font.getWidth(text, size);
        double height_of_text = font.getHeight(text, size);

        // Calculate alignment position
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
        // Scale the outline points based on the desired size
        float scale = font.getNearestSize(size) / size;
        float sizeIndex = font.getIndexForSize(size);
        
        float currentX = x;
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
			Character& character = font.characters[sizeIndex][*c];
			float x_offset = character.Bearing.x * scale;
			float y_offset = character.Bearing.y * scale;
			auto vertices = character.Vertices;
			for (auto& vertex : vertices.getVertexList()) {
				vertex.x = vertex.x * scale + currentX;
				vertex.y = y - vertex.y * scale;
			}
			ShapeManager::instance().drawTriangle(color, vertices);
			// Advance the cursor for the next character
			currentX += (character.Advance >> 6) * scale; // Bitshift by 6 to convert from 1/64th to pixels
        }
    }
}