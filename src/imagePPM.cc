#include "imagePPM.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <math.h>

ImagePPM::ImagePPM(const unsigned int _width, 
      const unsigned int _height, const Rgb &c) : 
   width(_width), height(_height)
{
   pixels = new Rgb[width * height];
   for (unsigned int i = 0; i < width * height; ++i)
      pixels[i] = c;
}

// create image object from stdin
ImagePPM::ImagePPM()
{
   std::istream *is = &std::cin;
   try
   {
      if (is->fail())
         throw("Can't open input stream");

      std::string header;
      unsigned int w, h, b;
      *is >> header;
      if (strcmp(header.c_str(), "P6") != 0)
         throw("Can't read input file");

      this->read_comments(is);

      *is >> w >> h >> b;
      this->width = w;
      this->height = h;
      this->pixels = new ImagePPM::Rgb[w * h];
      is->ignore(256, '\n'); // saute les lignes vides
      unsigned char pixel[3];
      // converti chaque pixel en float
      for (unsigned int i = 0; i < w * h; ++i)
      {
         is->read(reinterpret_cast<char *>(pixel), 3);
         this->pixels[i].r = pixel[0] / 255.0;
         this->pixels[i].g = pixel[1] / 255.0;
         this->pixels[i].b = pixel[2] / 255.0;
      }
   }
   catch (const char* err)
   {
      fprintf(stderr, "%s\n", err);
   }
}

ImagePPM::ImagePPM(const char* filename)
{
   std::ifstream ifs;
   ifs.open(filename, std::ios::binary);
   try
   {
      if (ifs.fail())
         throw("Can't open input file");

      std::string header;
      unsigned int w, h, b;
      ifs >> header;
      if (strcmp(header.c_str(), "P6") != 0)
         throw("Can't read input file");

      this->read_comments(&ifs);

      ifs >> w >> h >> b;
      this->width = w;
      this->height = h;
      this->pixels = new ImagePPM::Rgb[w * h];
      ifs.ignore(256, '\n'); // saute les lignes vides
      unsigned char pixel[3];
      // converti chaque pixel en float
      for (unsigned int i = 0; i < w * h; ++i)
      {
         ifs.read(reinterpret_cast<char *>(pixel), 3);
         this->pixels[i].r = pixel[0] / 255.0;
         this->pixels[i].g = pixel[1] / 255.0;
         this->pixels[i].b = pixel[2] / 255.0;
      }
      ifs.close();
   }
   catch (const char* err)
   {
      fprintf(stderr, "%s\n", err);
      ifs.close();
   }
}

// print image file on stdout
void ImagePPM::print() const 
{
   if (this->width == 0 || this->height == 0)
   {
      fprintf(stderr, "Can't print an empty image\n");
      return;
   }
   std::ostream *os = &std::cout;
   try
   {
      if (os->fail())
         throw("Can't open output stream");
      *os << "P6\n";
      *os << "# generated with libimagePPM\n";
      *os << this->width << " " << this->height << "\n255\n";
      unsigned char r, g, b;
      // clamp et converti les pixles en octet
      for (unsigned int i = 0; i < this->width * this->height; ++i)
      {
         r = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].r) * 255);
         g = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].g) * 255);
         b = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].b) * 255);
         *os << r << g << b;
      }
   }
   catch (const char* err)
   {
      fprintf(stderr, "%s\n", err);
   }
}

void ImagePPM::save(const char* filename) const 
{
   if (this->width == 0 || this->height == 0)
   {
      fprintf(stderr, "Can't save an empty image\n");
      return;
   }
   std::ofstream ofs;
   try
   {
      ofs.open(filename, std::ios::binary);
      if (ofs.fail())
         throw("Can't open output file");
      ofs << "P6\n";
      ofs << "# generated with libimagePPM\n";
      ofs << this->width << " " << this->height << "\n255\n";
      unsigned char r, g, b;
      // clamp et converti les pixles en octet
      for (unsigned int i = 0; i < this->width * this->height; ++i)
      {
         r = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].r) * 255);
         g = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].g) * 255);
         b = static_cast<unsigned char>(std::min(1.0f, 
                  this->pixels[i].b) * 255);
         ofs << r << g << b;
      }
      ofs.close();
   }
   catch (const char* err)
   {
      fprintf(stderr, "%s\n", err);
      ofs.close();
   }
}

void ImagePPM::read_comments(std::istream* is)
{
   // ignore comments
   is->get();
   std::string comment;
   while (is->peek() == '#')
   {
      std::getline(*is, comment);
      //fprintf(stderr, "Comment : %s\n", comment.c_str());
   }
}

void ImagePPM::read_comments(std::ifstream* ifs)
{
   // ignore comments
   ifs->get();
   std::string comment;
   while (ifs->peek() == '#')
   {
      std::getline(*ifs, comment);
      //fprintf(stderr, "Comment : %s\n", comment.c_str());
   }
}
