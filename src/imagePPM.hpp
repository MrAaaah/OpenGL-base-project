#ifndef IMAGEPPM_H
#define IMAGEPPM_H

#include <fstream>
#include <complex>

class ImagePPM
{
public:
    struct Rgb
    {
        float r, g, b;
        Rgb() : r(0.0f), g(0.0f), b(0.0f) {}
        Rgb(float c) : r(c), g(c), b(c) {}
        Rgb(float _r, float _g, float _b) : r(_r), g(_g), 
            b(_b) {}
    };

    unsigned int width, height;
    Rgb* pixels;

    ImagePPM(); 

    ImagePPM(const unsigned int _width, const unsigned int _height,
            const Rgb &c);

    ImagePPM(const char* filename);

    ~ImagePPM() 
    {
        delete [] pixels;
    }

    const Rgb& operator[] (const unsigned int i) const 
    {
        return pixels[i];
    }

    Rgb& operator[] (const unsigned int i)
    {
        return pixels[i];
    }

    void print() const;

    void save(const char* filename) const;

private:
    void read_comments(std::istream*);
    void read_comments(std::ifstream*);
};

#endif // IMAGEPPM_H
