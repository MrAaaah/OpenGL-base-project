#ifndef IMAGEPPM_H
#define IMAGEPPM_H

#include <fstream>
#include <complex>

class ImagePPM
{
public:
    struct Rgb
    {
        std::complex<double> r, g, b;
        Rgb() : r( 0.0, 0.0 ), g( 0.0, 0.0 ), b( 0.0, 0.0 ) {}
        Rgb( double c ) : r( c, 0.0 ), g( c, 0.0 ), b( c, 0.0 ) {}
        Rgb( double _r, double _g, double _b ) : r( _r, 0.0 ), g( _g, 0.0 ), 
            b( _b, 0.0 ) {}
    };

    unsigned int width, height;
    Rgb* pixels;

    ImagePPM(); 

    ImagePPM( const unsigned int _width, const unsigned int _height,
            const Rgb &c );

    ImagePPM( const char* filename );

    ~ImagePPM() 
    {
        delete [] pixels;
    }

    const Rgb& operator[] ( const unsigned int i ) const 
    {
        return pixels[ i ];
    }

    Rgb& operator[] ( const unsigned int i )
    {
        return pixels[ i ];
    }

    void print() const;

    void save( const char* filename ) const;

private:
    void read_comments( std::istream* );
    void read_comments( std::ifstream* );
};

#endif // IMAGEPPM_H
