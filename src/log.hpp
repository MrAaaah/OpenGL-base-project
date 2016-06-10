#ifndef LOG_H_
#define LOG_H_

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"

bool restartGlLog();
bool glLog(const char* message, ...);
bool glLogErr(const char* message, ...);
void glfwErrorCallback(int error, const char* description);
void _print_shader_info_log (GLuint shader_index);

#endif
