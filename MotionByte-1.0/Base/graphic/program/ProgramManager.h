#pragma once
#include "graphic/window/GlwfManager.h"
namespace MotionByte
{
	class ProgramManager
	{
	private:
	public:
		static GLuint createCompiledProgram(const char* vertex, const  char* fragment);
	};
}