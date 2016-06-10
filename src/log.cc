#include "log.hpp"

#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"

bool restartGlLog() {
   FILE* file = fopen(GL_LOG_FILE, "w");
   if (!file) {
      std::cerr << "Error: Can't open log file for writing: GL_LOG_FILE" << 
         std::endl;
      return false;
   }
   time_t now = time(NULL);
   char* date = ctime(&now);
   fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
   fclose(file);
   return true;
}

bool glLog(const char* message, ...) {
   va_list argptr;
   FILE* file = fopen(GL_LOG_FILE, "a");
   if (!file) {
      std::cerr << "Error: Can't open log file for appending: GL_LOG_FILE" << 
         std::endl;
      return false;
   }
   va_start(argptr, message);
   vfprintf(file, message, argptr);
   va_end(argptr);
   fclose(file);
   return true;
}

bool glLogErr(const char* message, ...) {
   va_list argptr;
   FILE* file = fopen(GL_LOG_FILE, "a");
   if (!file) {
      std::cerr << "Error: Can't open log file for appending: GL_LOG_FILE" << 
         std::endl;
      return false;
   }
   va_start(argptr, message);
   vfprintf(file, message, argptr);
   va_end(argptr);
   va_start(argptr, message);
   vfprintf(stderr, message, argptr);
   va_end(argptr);
   fclose(file);
   return true;
}

void glfwErrorCallback(int error, const char* description) {
  glLogErr("GLFW ERROR: code %i msg: %s\n", error, description);
}

void _print_shader_info_log (GLuint shader_index) {
  int max_length = 2048;
  int actual_length = 0;
  char log[2048];
  glGetShaderInfoLog (shader_index, max_length, &actual_length, log);
  printf ("shader info log for GL index %u:\n%s\n", shader_index, log);
}


