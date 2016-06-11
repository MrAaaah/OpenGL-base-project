#include <GL/glew.h>
#include <cmath>

struct Sphere
{
   GLuint vbos[2];
   GLuint vao;
   unsigned int nb_vert;
   unsigned int nb_tri;
   float * interleaved_data; // x, y, z, nx, ny, nz, tx, ty, tz,
                             // btx, bty, btz, u, v
   unsigned int * indices;

   Sphere(unsigned int lat_bands, unsigned int lon_bands, 
         float radius)
   {
      this->nb_vert = (lat_bands + 1) * (lon_bands + 1);
      this->nb_tri = 2 * lat_bands * lon_bands;
      this->interleaved_data = new float[11 * this->nb_vert];
      this->indices = new unsigned int[3 * this->nb_tri];

      unsigned int id = 0;
      for (unsigned int lat = 0 ; lat <= lat_bands ; lat++)
      {
         double theta = lat * M_PI / lat_bands;
         double sin_theta = std::sin(theta);
         double cos_theta = std::cos(theta);

         for (unsigned int lon = 0 ; lon <= lon_bands ; lon++)
         {
            double phi = lon * 2 * M_PI / lon_bands;
            double sin_phi = std::sin(phi);
            double cos_phi = std::cos(phi);
            
            float nx = cos_phi * sin_theta;
            float ny = cos_theta;
            float nz = sin_phi * sin_theta;

            this->interleaved_data[id++] = radius * nx; // x
            this->interleaved_data[id++] = radius * ny; // y
            this->interleaved_data[id++] = radius * nz; // z

            this->interleaved_data[id++] = nx; // nx
            this->interleaved_data[id++] = ny; // ny
            this->interleaved_data[id++] = nz; // nz

            id += 3;

            this->interleaved_data[id++] = std::atan2(nz, nx) / (2.0 * M_PI) + 0.5f; // u
            this->interleaved_data[id++] = 0.5 - std::asin(ny) / M_PI; // v
         }
      }

      // compute tangents
      for (unsigned int i = 0 ; i < nb_vert ; i++)
      {
         int offset = (i + 1) % lat_bands == 0 ? 1 - lat_bands : 1;
         glm::vec3 p1 = glm::vec3(
            this->interleaved_data[i * 11 + 0],
            this->interleaved_data[i * 11 + 1],
            this->interleaved_data[i * 11 + 2]
         );
         glm::vec3 p2 = glm::vec3(
            this->interleaved_data[(i + offset) * 11 + 0],
            this->interleaved_data[(i + offset) * 11 + 1],
            this->interleaved_data[(i + offset) * 11 + 2]
         );
         glm::vec3 tangent = glm::normalize(p2 - p1);
      
         this->interleaved_data[i * 11 + 6] =  tangent.x;
         this->interleaved_data[i * 11 + 7] =  tangent.y;
         this->interleaved_data[i * 11 + 8] =  tangent.z;
      }

      
      id = 0;
      for (unsigned int lat = 0 ; lat < lat_bands ; lat++)
      {
         for (unsigned int lon = 0 ; lon < lon_bands ; lon++)
         {
            unsigned int first = lat * (lon_bands  + 1) + lon;
            unsigned int second = first + lon_bands + 1;

            this->indices[id++] = first;
            this->indices[id++] = first + 1;
            this->indices[id++] = second;

            this->indices[id++] = second;
            this->indices[id++] = first + 1;
            this->indices[id++] = second + 1;
         }
      }

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

   ~Sphere()
   {
      delete this->interleaved_data;
      delete this->indices;
   }
};
