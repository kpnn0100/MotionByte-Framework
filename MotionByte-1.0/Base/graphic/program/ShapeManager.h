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
		/**
		 * * Draw a triangle strip using the provided color and vertex buffer.
		 * * @param color The color to use for the triangle strip.
		 * * @param Buffer The vertex buffer containing the vertices for the triangle strip.
		 * * This function prepares the vertex buffer and then draws the triangle strip using OpenGL.
		 * 
		 * Example of a triangle strip:
		 * 
		 *   v0 _______ v2
		 *    |      / |\
		 *    |    /   |  \
		 *    |  /     |    \ 
		 *   v1 -------v3----v4
		 * 
		 * The vertices are connected in the order: v0, v1, v2, v3.
		 */
		void drawTriangleStrip(Color color, VertexList Buffer);

		/**
		 * * Draw a triangle fan using the provided color and vertex buffer.
		 * * @param color The color to use for the triangle fan.
		 * * @param Buffer The vertex buffer containing the vertices for the triangle fan.
		 * * This function prepares the vertex buffer and then draws the triangle fan using OpenGL.
		 * 
		 * Example of a triangle fan:
		 * 
		 *       v0
		 *      / | \
		 *     /  |  \
		 *    /   |   \
		 *   v1---v2---v3
		 */
		void drawTriangleFan(Color color, VertexList Buffer);
		/**
		 * * Draw a quad using the provided color and vertex buffer.
		 * * @param color The color to use for the quad.
		 * * @param Buffer The vertex buffer containing the vertices for the quad.
		 * * This function prepares the vertex buffer and then draws the quad using OpenGL.
		 * 
		 * Example of a quad:
		 * 
		 *   v0 ------- v1
		 *    |         |
		 *    |         |
		 *    |         |
		 *   v3 ------- v2
		 * 	group 4 vertices to one polygon
		 */
		void drawQuad(Color color, VertexList Buffer);

		/**
		 * * Draw a triangle using the provided color and vertex buffer.
		 * * @param color The color to use for the triangle.
		 * * @param Buffer The vertex buffer containing the vertices for the triangle.
		 * * This function prepares the vertex buffer and then draws the triangle using OpenGL.
		 * 
		 * Example of a triangle:
		 * 
		 *       v0
		 *      / \
		 *     /   \
		 *    v1----v2
		 */
		void drawTriangle(Color color, VertexList Buffer);

		/**
		 * * Draw a polygon using the provided color and vertex buffer.
		 * * @param color The color to use for the polygon.
		 * * @param Buffer The vertex buffer containing the vertices for the polygon.
		 * * This function prepares the vertex buffer and then draws the polygon using OpenGL.
		 * 
		 * Example of a polygon:
		 * 
		 *       v0 ---- v1
		 *      /  \      \
		 *     /    \      \
		 *    v4 ----v3 ----v2
		 */
		void drawPolygon(Color color, VertexList Buffer);
	};
}