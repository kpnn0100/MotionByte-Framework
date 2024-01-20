#pragma once
#include "../Window/GlwfManager.h"
#include "ProgramManager.h"
#include "Vertex.h"
#include "../Segment/Rectangle.h"
#include "Color.h"
namespace MotionByte
{

	class ShapeManager
	{
	private:
		GLuint vertexBuffer;                      ///< OpenGL vertex buffer.                     ///< OpenGL color buffer.
		GLuint mProgram;
		float mWidth, mHeight;
		ShapeManager();
	public:
		static ShapeManager& instance();
		void onWindowSizeChanged(int width, int height);
		void setBoundAndOffset(Rectangle bound, Point offset);
		void prepareBuffer(VertexList Buffer, Color color);
	};
}