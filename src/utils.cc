#include "utils.hpp"

#include <iostream>
#include <fstream>

std::string readFile(const char* filepath) {
   std::string content;
   std::ifstream fileStream(filepath, std::ios::in);
   if (!fileStream.is_open()) {
      std::cerr << "Can't read file: " << filepath << std::endl;
      return "";
   }

   std::string line = "";
   while (!fileStream.eof()) {
      std::getline(fileStream, line);
      content.append(line + "\n");
   }

   fileStream.close();
   return content;
}
