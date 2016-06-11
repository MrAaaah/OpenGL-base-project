#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <assert.h>
#include <complex>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include "window.hpp" 
#include "utils.hpp"
#include "log.hpp"
#include "shader.hpp"
#include "imagePPM.hpp"
#include "camera.hpp"

#include "sphere.hpp" 
#include "plane.hpp" 

#define M_2PI M_PI*2.0f
#define CAMERA_LEFT_KEY GLFW_KEY_A
#define CAMERA_RIGHT_KEY GLFW_KEY_D
#define CAMERA_FORWARD_KEY GLFW_KEY_W
#define CAMERA_BACK_KEY GLFW_KEY_S
#define CAMERA_MODE_KEY GLFW_KEY_SPACE

bool move_left = false;
bool move_right = false;
bool move_forward = false;
bool move_backward = false;
bool command_mode = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, 
      int mods)
{
   if (key == CAMERA_LEFT_KEY && action == GLFW_PRESS)
      move_left = true;
   if (key == CAMERA_LEFT_KEY && action == GLFW_RELEASE)
      move_left = false;
   if (key == CAMERA_RIGHT_KEY && action == GLFW_PRESS)
      move_right = true;
   if (key == CAMERA_RIGHT_KEY && action == GLFW_RELEASE)
      move_right = false;
   if (key == CAMERA_FORWARD_KEY && action == GLFW_PRESS)
      move_forward = true;
   if (key == CAMERA_FORWARD_KEY && action == GLFW_RELEASE)
      move_forward = false;
   if (key == CAMERA_BACK_KEY && action == GLFW_PRESS)
      move_backward = true;
   if (key == CAMERA_BACK_KEY && action == GLFW_RELEASE)
      move_backward = false;

   if (key == CAMERA_MODE_KEY && action == GLFW_PRESS)
   {
      command_mode = !command_mode; 
      if (command_mode)
      {
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      }
      else
      {
         glfwSetCursorPos(window, 0, 0);
         glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      }
   }
}

int main (int argc, char** argv) {
   assert(restartGlLog());

   GLFWwindow * window = init_window("Wahoo", 700, 700, key_callback);

   //Sphere sphere(300, 300, 1.5f);
   Plane sphere;

   ImagePPM texture("ressources/checker.ppm");
   //
   // load texture on the GPU
   GLuint texture_id;
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
         GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
         GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, 
         texture.height, 0, GL_RGB, GL_FLOAT, texture.pixels);

   glGenerateMipmap(GL_TEXTURE_2D);

   // normal map
   ImagePPM normal_map("ressources/wave_normal_map.ppm");
   //
   // load texture on the GPU
   GLuint normal_map_id;
   glGenTextures(1, &normal_map_id);
   glBindTexture(GL_TEXTURE_2D, normal_map_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
         GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
         GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, normal_map.width, 
         normal_map.height, 0, GL_RGB, GL_FLOAT, normal_map.pixels);

   glGenerateMipmap(GL_TEXTURE_2D);

   // height map
   ImagePPM height_map("ressources/wave_height_map.ppm");
   //
   // load texture on the GPU
   GLuint height_map_id;
   glGenTextures(1, &height_map_id);
   glBindTexture(GL_TEXTURE_2D, height_map_id);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
         GL_LINEAR_MIPMAP_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 
         GL_LINEAR);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, height_map.width, 
         height_map.height, 0, GL_RGB, GL_FLOAT, height_map.pixels);

   glGenerateMipmap(GL_TEXTURE_2D);

   glBindTexture(GL_TEXTURE_2D, 0);


   // init shaders
   GLuint vs = loadAndCompileShader(GL_VERTEX_SHADER, "src/vs.glsl");
   GLuint fs = loadAndCompileShader(GL_FRAGMENT_SHADER, "src/fs.glsl");

   GLuint shader_program = glCreateProgram();
   glAttachShader(shader_program, fs);
   glAttachShader(shader_program, vs);
   glBindAttribLocation(shader_program, 0, "position");
   glBindAttribLocation(shader_program, 1, "normal");
   glBindAttribLocation(shader_program, 2, "tangent");
   glBindAttribLocation(shader_program, 3, "uv");
   glLinkProgram(shader_program);

   glUseProgram(shader_program);
   GLint mvp_uniform = glGetUniformLocation(shader_program, "mvp");
   GLint view_pos_uniform = glGetUniformLocation(shader_program, 
         "view_pos");
   GLint light_pos_uniform = glGetUniformLocation(shader_program, 
         "light_pos");
   GLint resolution_uniform = glGetUniformLocation(shader_program, 
         "resolution_window");
   GLint height_scale_uniform = glGetUniformLocation(shader_program,
         "height_scale");
   GLint num_parallax_uniform = glGetUniformLocation(shader_program,
         "num_parallax_layers");
   glUniform1i(glGetUniformLocation(shader_program, "texture_sampler"), 0);
   glUniform1i(glGetUniformLocation(shader_program, "normal_map_sampler"), 1);
   glUniform1i(glGetUniformLocation(shader_program, "height_map_sampler"), 2);

   // setup matrices
   Camera camera(window, glm::vec3(4.0f, 3.0f, 4.0f), 80);

   glm::vec3 light_pos(4.0, 4.0, 4.0);
   float height_scale = 0.30f;
   int num_parallax_layers = 20;

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   float last_time = glfwGetTime(), deltatime = 0;
   glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
   while (!glfwWindowShouldClose(window)) {
      // update other events like input handling 
      glfwPollEvents();


      ImGui_ImplGlfwGL3_NewFrame();
      // 1. Show a simple window
      {
         bool show_window = true;
         ImGui::Begin("Stats", &show_window, 
               ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("Application average \n %.3f ms/frame (%.1f FPS)", 
               1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
         ImGui::Image((void*)(intptr_t)texture_id, ImVec2(256, 256), 
               ImVec2(0,0), ImVec2(1,1), 
               ImColor(255,255,255,255), ImColor(255,255,255,128));
         ImGui::SliderFloat3("Light position", &light_pos[0], -10.0, 10.0, "%.3f", 1);
         ImGui::SliderFloat("Height scale", &height_scale, 0.0, 2.0, "%.3f", 1);
         ImGui::SliderInt("Num parallax layers", &num_parallax_layers, 1, 50, NULL);
         ImGui::End();
      }

      float current_time = glfwGetTime();
      deltatime = current_time - last_time;
      last_time = current_time;

      glm::vec2 delta_translate(
            move_right - move_left,
            move_forward - move_backward
            );
      camera.update_camera(window, deltatime, delta_translate, command_mode);

      camera.display_hud();
      //ImGui::ShowTestWindow();
      //
      // Rendering
      int display_w, display_h;
      glfwGetFramebufferSize(window, &display_w, &display_h);
      glViewport(0, 0, display_w, display_h);
      // wipe the drawing surface clear
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // magnitudes plot
      glUseProgram(shader_program);
      // update uniforms
      glm::mat4 mvp_matrix = camera.get_projection_matrix() * 
         camera.get_view_matrix();// * object.model_matrix;
      glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp_matrix[0][0]);
      glUniform3fv(view_pos_uniform, 1, &camera.position[0]);
      glUniform3fv(light_pos_uniform, 1, &light_pos[0]);
      glUniform2f(resolution_uniform, display_w, display_h);
      glUniform1f(height_scale_uniform, height_scale);
      glUniform1i(num_parallax_uniform, num_parallax_layers);

      glBindVertexArray(sphere.vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.vbos[1]);

      glEnable(GL_TEXTURE_2D);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_id);

      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, normal_map_id);

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, height_map_id);

      glDrawElements(GL_TRIANGLES, sphere.nb_tri * 3, GL_UNSIGNED_INT, (void*)0);
      //
      //
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      ImGui::Render();
      glfwSwapBuffers(window);

      // animate
      light_pos = glm::vec3(std::cos(current_time) * 4.0, 4.0, std::sin(current_time) * 4.0);
   }

   // close GL context and any other GLFW resources
   ImGui_ImplGlfwGL3_Shutdown();
   glfwTerminate();
   return 0;
}
