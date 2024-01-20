#include "MotionByte-1.0/Base/Window/Window.h"
const char* textVertexSource1 = R"(
	#version 460 core
	layout (location = 3) in vec4 vertex; // <vec2 pos, vec2 tex>
	layout (location = 2) uniform mat4 projection;

	out vec2 TexCoords;

	void main()
	{
		gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
		TexCoords = vertex.zw;
	}  
)";

const char* textFragmentSource = R"(
	#version 460 core
	in vec2 TexCoords;
	out vec4 color;

	layout (binding = 0) uniform sampler2D text;
	layout (location = 6) uniform vec4 textColor;

	void main()
	{    
		color = textColor * texture(text, TexCoords).r;
	} 
)";

const char* vertexShaderSource2 = R"(
    #version 460 core

    // Input vertex data, different for all executions of this shader.
    layout(location = 0) in vec2 vertexPosition_modelspace;
    layout(location = 1) in vec4 vertexColor;
	layout (location = 2) uniform mat4 projection;
    layout (location = 3) uniform vec4 bound;
    layout (location = 4) uniform vec2 offset;
    // Output data ; will be interpolated for each fragment.
    out vec4 fragmentColor;
    // Values that stay constant for the whole mesh.

    void main(){	
		gl_Position = vec4(vertexPosition_modelspace+offset, 0.0, 1.0);
         gl_Position = projection * gl_Position;
        gl_Position.y = -gl_Position.y;
        //gl_Position = vec4(vertexPosition_modelspace, 0.0, 1.0);
	    // The color of each vertex will be interpolated
	    // to produce the color of each fragment
	    fragmentColor = vertexColor;
    }
)";

const char* fragmentShaderSource2 = R"(
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
GLuint Compile_Shaders(const char* vertex, const  char* fragment);
using namespace MotionByte;
GLuint vertexArrayID;
GLuint colorBuffer;
GLuint vertexBuffer;
GLuint shader;
GLuint shader2;
void drawRectangle(Color color, Rectangle bound)
{
	VertexList vertices;
	std::vector<float> colors;  // Add color information
	for (int i = 0; i <= Rectangle::CornerCount; ++i) {
		Point currentCorner = bound.getCorner(i % Rectangle::CornerCount);
		float x_outer = currentCorner.getX().getValue();
		float y_outer = currentCorner.getY().getValue();
		vertices.addVertex(x_outer, y_outer);
		colors.push_back((double)color.getRed()); // R
		colors.push_back((double)color.getGreen()); // G
		colors.push_back((double)color.getBlue()); // B
		colors.push_back((double)color.getAlpha()); // Alpha
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.sizeInFloat() * sizeof(float), vertices.toBufferArray(), GL_DYNAMIC_DRAW);


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_DYNAMIC_DRAW);

	// Use the correct layout location for the color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Draw
	glDrawArrays(GL_QUADS, 0, vertices.sizeInFloat() / 2);
	
	// Clean up
}
struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

void APIENTRY GLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
	printf("%d: %s\n", id, msg);
}

int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(800, 600, "YEAH", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	glewInit();
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLDebugMessageCallback, NULL);
	glViewport(0, 0, 800, 600);



	shader = Compile_Shaders(textVertexSource1, textFragmentSource);
	glUseProgram(shader);
	shader2 = Compile_Shaders(vertexShaderSource2, fragmentShaderSource2);
	glUseProgram(shader2);

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	GLint projectionLocation = glGetUniformLocation(shader2, "projection");
	GLint offsetLocation = glGetUniformLocation(shader2, "offset");
	glUniform2f(4, 1,2);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(projection));


	MotionByte::FontManager::instance();
	std::string text("Hello OpenGL");

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		GLuint projectionLocation = glGetUniformLocation(shader2, "projection");
		//glUseProgram(shader2);
		////glDisable(GL_CULL_FACE);
		//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//drawRectangle(MotionByte::Color(100, 200, 125, 255),
		//	Rectangle(Point(20, 60), 200.0, 90160.0));
		glUseProgram(shader);
		//glEnable(GL_CULL_FACE);
		glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(projection));
		GLfloat x = 50.0f;
		GLfloat y = 300.0f;
		GLfloat scale = 1.0f;
		MotionByte::FontManager::instance().RenderText(
			text, x, y, scale, MotionByte::Color(100, 200, 125, 255));
		//glUseProgram(shader2);
		////glDisable(GL_CULL_FACE);
		//glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		//drawRectangle(MotionByte::Color(100, 200, 125, 255),
		//	Rectangle(Point(70, 260), 200.0, 90160.0));
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
	printf("%d", glGetError());
	return 0;
}



GLuint Compile_Shaders(const char* vertex, const  char* fragment) {

	GLuint shader_programme = glCreateProgram();

	GLuint vs, tcs, tes, gs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex, NULL);
	glCompileShader(vs);

	glAttachShader(shader_programme, vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment, NULL);
	glCompileShader(fs);


	glAttachShader(shader_programme, fs);
	glLinkProgram(shader_programme);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return shader_programme;
}