#ifndef PLANE_HPP_
#define PLANE_HPP_

#include <GL/glew.h>
#include <cmath>

struct Plane
{
   GLuint vbos[2];
   GLuint vao;
   unsigned int nb_vert;
   unsigned int nb_tri;
   float * interleaved_data; // x, y, z, nx, ny, nz, tx, ty, tz,
                             // btx, bty, btz, u, v
   unsigned int * indices;

   Plane()
   {
      this->nb_vert = 4;
      this->nb_tri = 2;
      this->interleaved_data = new float[11 * this->nb_vert];
      this->indices = new unsigned int[3 * this->nb_tri];
      
      unsigned int id = 0;
      // v0
      this->interleaved_data[id++] = -1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 0.0;


      // v1
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;


      // v2
      this->interleaved_data[id++] = -1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = -1.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;


      // v3
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = -1.0;

      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 0.0;
      this->interleaved_data[id++] = 0.0;

      this->interleaved_data[id++] = 1.0;
      this->interleaved_data[id++] = 1.0;


      
      id = 0;
      this->indices[id++] = 0;
      this->indices[id++] = 1;
      this->indices[id++] = 2;

      this->indices[id++] = 1;
      this->indices[id++] = 3;
      this->indices[id++] = 2;

      glGenBuffers(2, this->vbos);
      glBindBuffer(GL_ARRAY_BUFFER, this->vbos[0]);
      glBufferData(GL_ARRAY_BUFFER, 
         11 * this->nb_vert * sizeof(float), this->interleaved_data, 
         GL_STATIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);


      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbos[1]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
         3 * this->nb_tri * sizeof(unsigned int), this->indices, 
         GL_STATIC_DRAW);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

      //init VAO
      glGenVertexArrays(1, &this->vao);
      glBindVertexArray(this->vao);
      glBindBuffer(GL_ARRAY_BUFFER, this->vbos[0]);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
         11 * sizeof(float), 0);

      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
         11 * sizeof(float), (void *)(3 * sizeof(float)));

      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 
         11 * sizeof(float), (void *)(6 * sizeof(float)));

      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 
         11 * sizeof(float), (void *)(9 * sizeof(float)));
   }

   ~Plane()
   {
      delete this->interleaved_data;
      delete this->indices;
   }
};

#endif // PLANE_HPP_
