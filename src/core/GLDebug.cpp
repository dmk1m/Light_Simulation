#include <core/GLDebug.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// The bundled GLAD loader targets OpenGL 3.3 without GL_KHR_debug, so the
// handful of names needed here are declared locally and the entry point is
// loaded through GLFW.
namespace {
	constexpr GLenum DEBUG_OUTPUT = 0x92E0;
	constexpr GLenum DEBUG_OUTPUT_SYNCHRONOUS = 0x8242;
	constexpr GLenum DEBUG_SEVERITY_HIGH = 0x9146;
	constexpr GLenum DEBUG_SEVERITY_MEDIUM = 0x9147;
	constexpr GLenum DEBUG_SEVERITY_LOW = 0x9148;
	constexpr GLenum DEBUG_SEVERITY_NOTIFICATION = 0x826B;
	constexpr GLenum CONTEXT_FLAGS = 0x821E;
	constexpr GLint CONTEXT_FLAG_DEBUG_BIT = 0x00000002;

	using DebugProc = void (APIENTRY*)(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar* message, const void* userParam);
	using DebugMessageCallbackFn = void (APIENTRY*)(DebugProc callback, const void* userParam);

	const char* severityName(GLenum severity)
	{
		switch (severity)
		{
		case DEBUG_SEVERITY_HIGH:   return "high";
		case DEBUG_SEVERITY_MEDIUM: return "medium";
		case DEBUG_SEVERITY_LOW:    return "low";
		default:                    return "info";
		}
	}

	void APIENTRY onDebugMessage(GLenum /*source*/, GLenum /*type*/, GLuint id, GLenum severity,
		GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
	{
		// Notifications (buffer placement hints and the like) are just noise.
		if (severity == DEBUG_SEVERITY_NOTIFICATION)
		{
			return;
		}
		std::cerr << "[GL " << severityName(severity) << " #" << id << "] " << message << std::endl;
	}
}

bool installGLDebugOutput()
{
	GLint flags = 0;
	glGetIntegerv(CONTEXT_FLAGS, &flags);
	if (!(flags & CONTEXT_FLAG_DEBUG_BIT))
	{
		return false;
	}

	GLint major = 0, minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	const bool hasCoreDebug = major > 4 || (major == 4 && minor >= 3);
	if (!hasCoreDebug && !glfwExtensionSupported("GL_KHR_debug"))
	{
		return false;
	}

	auto debugMessageCallback = reinterpret_cast<DebugMessageCallbackFn>(
		glfwGetProcAddress("glDebugMessageCallback"));
	if (!debugMessageCallback)
	{
		return false;
	}

	glEnable(DEBUG_OUTPUT);
	glEnable(DEBUG_OUTPUT_SYNCHRONOUS);  // report on the offending call's thread/stack
	debugMessageCallback(onDebugMessage, nullptr);
	return true;
}
