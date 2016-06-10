#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <cmath>

#include <GLFW/glfw3.h> // GLFW helper library

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include "imgui/imgui.h"
#include "imgui_impl_glfw_gl3.hpp"

struct Camera {
   glm::mat4 projection_matrix;
   glm::mat4 view_matrix;
   glm::vec3 position;
   glm::vec3 look_at;
   glm::vec3 up;
   glm::vec2 last_mouse_pos;
   float ratio, fov;
   float phi, theta;
   float mouse_speed, move_speed;
   bool need_view_matrix_update;

   Camera(GLFWwindow* window, glm::vec3 position, float fov)
   {
      int w_width, w_height;
      glfwGetWindowSize(window, &w_width, &w_height);

      this->position = position;
      this->look_at = glm::vec3(0, 0, 0);
      this->fov = fov;
      this->ratio = w_width / float(w_height);
      this->move_speed = 2.0f;
      this->mouse_speed = 0.1f;
      this->last_mouse_pos = glm::vec2(0, 0);
      this->up = glm::vec3(0, 1, 0);

      glm::vec3 view_dir = this->look_at - this->position;
      view_dir = glm::normalize(view_dir);

      this->phi = std::atan2(view_dir.z, view_dir.x);
      this->theta = std::acos(view_dir.y);
   }

   glm::mat4 get_projection_matrix()
   {
      this->projection_matrix = glm::perspective(
            float(2 * M_PI * this->fov / 360),           // fov
            this->ratio,  // ratio
            0.1f,          // near plane
            100.0f         // far plane
            );

      return this->projection_matrix;
   }

   glm::mat4 get_view_matrix()
   {
      if (this->need_view_matrix_update)
      {
         this->view_matrix = glm::lookAt(
               this->position, // camera position
               this->look_at, // target position
               this->up  // up vector
               ); 
      }

      return this->view_matrix;
   }

   void update_camera(GLFWwindow* window, float deltatime, 
         glm::vec2 delta_translate, bool block_cursor)
   {
      // get window's size
      int w_width, w_height;
      glfwGetWindowSize(window, &w_width, &w_height);
      glm::vec2 delta_cursor(0.0f);
      if (!block_cursor)
      {
         // get mouse position
         double mouse_pos_x, mouse_pos_y;
         glfwGetCursorPos(window, &mouse_pos_x, &mouse_pos_y);
         glm::vec2 mouse_pos(mouse_pos_x, mouse_pos_y);
         delta_cursor = mouse_pos - last_mouse_pos;
         last_mouse_pos = mouse_pos;
      }

      // compute new orientation
      this->phi += deltatime * delta_cursor.x * mouse_speed;
      this->theta += deltatime * delta_cursor.y * mouse_speed;

      glm::vec3 direction(
            std::sin(this->theta) * std::cos(this->phi),
            std::cos(this->theta),
            std::sin(this->theta) * std::sin(this->phi)
            );

      glm::vec3 right(
            std::cos(this->phi + M_PI / 2.0f),
            0,
            std::sin(this->phi + M_PI / 2.0f)
            );

      this->position += deltatime * right * delta_translate.x * move_speed;
      this->position += deltatime * direction * delta_translate.y * move_speed;

      this->up = glm::cross(right, direction);

      // update camera
      this->look_at = this->position + direction;
      this->need_view_matrix_update = true;
   }

   void display_hud()
   {
      {
         bool show_window = true;
         ImGui::Begin("Camera control", &show_window, 
               ImGuiWindowFlags_AlwaysAutoResize);
         ImGui::Text("Position: (%0.3f, %0.3f, %0.3f)", 
               this->position.x, this->position.y, this->position.z);
         ImGui::Text("Look at: (%0.3f, %0.3f, %0.3f)", 
               this->look_at.x, this->look_at.y, this->look_at.z);
         ImGui::SliderFloat("FOV", &(this->fov), 20.0f, 120.0f);
         ImGui::SliderFloat("Mouse speed", &(this->mouse_speed), 0.1f, 10.0f);
         ImGui::SliderFloat("Move speed", &(this->move_speed), 0.1f, 10.0f);
         ImGui::End();
      }
   }
};

#endif
