#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <GL/glew.h> 
#include <GLFW/glfw3.h>

GLFWwindow * init_window(const char * title, unsigned int width,
      unsigned int height, GLFWkeyfun key_callback_function = NULL);

#endif
