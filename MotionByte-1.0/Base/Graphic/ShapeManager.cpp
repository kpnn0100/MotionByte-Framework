#include "ShapeManager.h"
namespace MotionByte
{
        const char* vertexShaderSource = R"(
        #version 460 core

        // Input vertex data, different for all executions of this shader.
        layout(location = 0) in vec2 vertexPosition_modelspace;
	    layout (location = 1) uniform vec4 vertexColor;
	    layout (location = 2) uniform mat4 projection;
        layout (location = 3) uniform vec4 bound;
        layout (location = 4) uniform vec2 offset;
        // Output data ; will be interpolated for each fragment.
        out vec4 fragmentColor;
        // Values that stay constant for the whole mesh.

        void main(){	
            // bound.x <  gl_Position.x <bound.z
            float clampedX = clamp(vertexPosition_modelspace.x + offset.x, bound.x, bound.z);
            float clampedY = clamp(vertexPosition_modelspace.y + offset.y, bound.y, bound.w);
            gl_Position = vec4(clampedX,clampedY,0.0,1.0);
            gl_Position = projection * gl_Position;
            gl_Position.y = -gl_Position.y;
            //gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);
	        // The color of each vertex will be interpolated
	        // to produce the color of each fragment
	        fragmentColor = vertexColor;
        }
    )";

        const char* fragmentShaderSource = R"(
        #version 460 core

        // Interpolated values from the vertex shaders

        in vec4 fragmentColor;

        // Ouput data
        out vec4 color;

        void main(){

        // Output color = color specified in the vertex shader, 
        // interpolated between all 3 surrounding vertices
        color = fragmentColor;

        }
    )";

        ShapeManager::ShapeManager():
            mProgram(ProgramManager::createCompiledProgram
                (vertexShaderSource, fragmentShaderSource))
        {
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        }

        ShapeManager& ShapeManager::instance()
        {
            static ShapeManager instance;
            return instance;
        }

        void ShapeManager::onWindowSizeChanged(int width, int height)
        {
            mWidth = width;
            mHeight = height;
        }

        void ShapeManager::setBoundAndOffset(Rectangle bound, Point offset)
        {
            glUseProgram(mProgram);
            GLint boundLocation = glGetUniformLocation(mProgram, "bound");
            Point topLeft = bound.getCorner(bound.TopLeft);
            Point bottomRight = bound.getCorner(bound.BottomRight);
            glUniform4f(boundLocation, topLeft.getX(), topLeft.getY()
                , bottomRight.getX(), bottomRight.getY());
            GLint offsetLocation = glGetUniformLocation(mProgram, "offset");
            glUniform2f(offsetLocation, offset.getX(), offset.getY());

            glm::mat4 projection = glm::ortho(0.0f, mWidth, 0.0f, mHeight);
            GLint projectionLocation = glGetUniformLocation(mProgram, "projection");
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        }

        void ShapeManager::prepareBuffer(VertexList Buffer, Color color)
        {
            glUseProgram(mProgram);

            glUniform4f(1, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, Buffer.sizeInFloat() * sizeof(float), Buffer.toBufferArray(), GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        }

}