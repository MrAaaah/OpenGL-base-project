#include "opengl_helpers.hpp"

#include "imagePPM.hpp" 

GLuint ppm_to_gpu_texture(const char * image_path)
{
   ImagePPM texture(image_path);
   //
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

   glBindTexture(GL_TEXTURE_2D, 0);

   return texture_id;
}
