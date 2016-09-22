/**
 * \file image.cpp
 * \brief Implementation of class Image
 * \author B.BORGOBELLO
 */

#include "image.hpp"

using std::string ;
using boost::shared_ptr ;

/**
 * \param filename : the name of the .TGA file to import
 * into the project (for Bitmap textures)
 *
 * This function creates an Image, which is the representation
 * of a .TGA file with a matrix of pixels
 */
Image::Image(const std::string filename) {
    using namespace std;

    std::ifstream stream;
    stream.open(filename.c_str());
    if (!stream) {
        std::cout << std::endl << "Can't read the TGA file " << filename << " or doesn't exists" << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        unsigned int size_x=0;
        unsigned int size_y=0;
        char car;
        unsigned char ucar;

        stream.seekg(12);

        stream.get(car);
        ucar = car;
        size_x += ucar;
        stream.get(car);
        ucar = car;
        size_x += ucar*256;

        stream.get(car);
        ucar = car;
        size_y += ucar;
        stream.get(car);
        ucar = car;
        size_y += ucar*256;

        stream.get(car);
        stream.get(car);

        _res_x = size_x;
        _res_y = size_y;

        _matrix.resize(_res_x);
        for (int i = 0; i < _res_x; i++)
            _matrix.at(i).resize(_res_y);

        char r,g,b;
        boost::shared_ptr<Color> col;
        for (int j=_res_y-1; j >= 0 ; j--) {
            for (int i=0 ; i <_res_x; i++) {
                stream.get(b);
                stream.get(g);
                stream.get(r);
                _matrix[i][j] = col = boost::shared_ptr<Color>(new Color( (double)((unsigned char)r)/255.0 ,(double)((unsigned char)g)/255.0 ,(double)((unsigned char)b)/255.0));
            }
        }
        stream.close();
    }
}

/**
 * \brief Special byte conversion
 */
void Image::special_byte(int number, int * byte0, int * byte1, int * byte2, int * byte3)
{
    *byte0 = ((unsigned int)number << 24) >> 24;
    *byte1 = ((unsigned int)number << 16) >> 24;
    *byte2 = ((unsigned int)number << 8) >> 24;
    *byte3 = ((unsigned int)number) >> 24;
}

/**
 * \brief Method to save an Image class into .TGA
 *
 * Saves this Image in filename file (.TGA)
 *
 * \param filename : name of the file to save into, with extension
 */
void Image::save_to_TGA(const std::string& filename) const {
    std::ofstream stream;
    stream.open(filename.c_str());
    if (!stream) {
        std::cout << "Can't write the TGA file " << filename << std::endl;
        return;
    }
    else {
        // HEADER
        stream.put(0);
        stream.put(0);
        stream.put(2);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);
        stream.put(0);


        // resX
        int byte0, byte1, byte2, byte3;

        special_byte(_res_x, &byte0, &byte1, &byte2, &byte3);
        stream.put(byte0);
        stream.put(byte1);

        // resY
        special_byte(_res_y, &byte0, &byte1, &byte2, &byte3);
        stream.put(byte0);
        stream.put(byte1);

        stream.put(24);
        stream.put(0);

    // Storing DATA // Left to right, up to down
        for (int j = _res_y-1; j >= 0; j--) {
            for (int i = 0; i < _res_x;i++) {
            	Color c = *(_matrix[i][j]) ;
            	stream.put( (unsigned char)( 255.0*(c.get_b() < 1.0 ? c.get_b() : 1.0) ) );
                stream.put( (unsigned char)( 255.0*(c.get_g() < 1.0 ? c.get_g() : 1.0) ) );
                stream.put( (unsigned char)( 255.0*(c.get_r() < 1.0 ? c.get_r() : 1.0) ) );
            }
        }
        stream.close();
        return;
    }
}

/**
 * \brief Adds a color to a given position in the matrix
 *
 * Correponding pixel in the matrix receives the given color
 *
 * \param color : Pointer to the color to save
 * \param x, y : Position in the matrix of the pixel to modify
 */
void Image::add_color(boost::shared_ptr<Color> color, int x, int y) {
    _matrix[x][y] = color ;
}
