#pragma once

// Routes OpenGL driver messages (errors, performance and portability
// warnings) to std::cerr. Needs a debug context
// (GLFW_OPENGL_DEBUG_CONTEXT) whose driver supports GL_KHR_debug or
// OpenGL 4.3; otherwise it does nothing and returns false.
// Call after the context is current and GLAD is loaded.
bool installGLDebugOutput();
