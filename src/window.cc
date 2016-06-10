#include "window.hpp"

#include <cstdio>

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.hpp"

GLFWwindow * init_window(const char * title, unsigned int width,
      unsigned int height, GLFWkeyfun key_callback_function)
{
   // start GL context and O/S window using the GLFW helper library
   if (!glfwInit()) {
      fprintf(stderr, "ERROR: could not start GLFW3\n");
      return NULL;
   } 

   // uncomment these lines if on Apple OS X
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   GLFWwindow * window = glfwCreateWindow(width, height, 
         title, NULL, NULL);
   if (!window) {
      fprintf(stderr, "ERROR: could not open window with GLFW3\n");
      glfwTerminate();
      return NULL;
   }
   glfwMakeContextCurrent(window);

   // start GLEW extension handler
   glewExperimental = GL_TRUE;
   glewInit();

   // Setup ImGui binding
   ImGui_ImplGlfwGL3_Init(window, true);

   // set keyboard callback
   if (key_callback_function != NULL)
      glfwSetKeyCallback(window, key_callback_function);

   int w_width, w_height;
   glfwGetWindowSize(window, &w_width, &w_height);

   // get version info
   const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
   const GLubyte* version = glGetString(GL_VERSION); // version as a string
   printf("Renderer: %s\n", renderer);
   printf("OpenGL version supported %s\n", version);

   // tell GL to only draw onto a pixel if the shape is closer to the viewer
   glEnable(GL_DEPTH_TEST); // enable depth-testing
   glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

   return window;
}

