#include "Frame.h"
namespace pertyG
{
	void Frame::fillColor(Color color)
	{

		glClearColor(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
		glClear(GL_COLOR_BUFFER_BIT);
	}
    void Frame::drawSomething()
    {

        //// Example: Drawing a simple triangle
        //glBegin(GL_TRIANGLES);
        //glColor3f(1.0f, 0.0f, 0.0f);  // Red color
        //glVertex2f(-0.6f, -0.5f);
        //glColor3f(0.0f, 1.0f, 0.0f);  // Green color
        //glVertex2f(0.6f, -0.5f);
        //glColor3f(0.0f, 0.0f, 1.0f);  // Blue color
        //glVertex2f(0.0f, 0.6f);
        //glEnd();
    }
    void Frame::showBuffer()
    {
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
    }
}