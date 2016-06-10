#include "shader.hpp"

#include <stdio.h>

#include "utils.hpp"
#include "log.hpp"

GLuint loadAndCompileShader(GLuint shaderType, const char* filepath) {
   // read shader source file
   std::string shader_source = readFile(filepath);
   const char* shader_source_cc = shader_source.c_str();
   GLuint shader = glCreateShader(shaderType);
   glShaderSource(shader, 1, &shader_source_cc, NULL);
   glCompileShader(shader);
   // check for compile errors
   int params = -1;
   glGetShaderiv(shader, GL_COMPILE_STATUS, &params);
   if (GL_TRUE != params) {
      fprintf (stderr, "ERROR: GL shader index %i did not compile\n", shader);
      _print_shader_info_log(shader);
      return -1; // or exit or something
   }
   return shader;
}

