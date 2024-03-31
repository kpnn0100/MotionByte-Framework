#pragma once
#include "graphic/window/GraphicManager.h"
#include "ProgramManager.h"
#include "util/geometry/Vertex.h"
#include "util/geometry/Rectangle.h"
#include "util/geometry/Color.h"
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