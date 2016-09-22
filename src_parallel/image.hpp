#ifndef IMAGE_HPP_
#define IMAGE_HPP_

/**
 * \file image.hpp
 * \author B.BORGOBELLO
 * \brief Declaration of class Image
 */

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "color.hpp"

/**
 * \class Image
 * \brief Class representing an Image
 *
 * Image is a middle structure used to contains the
 * calculated colors during the rendering phase
 * It possesses methods to save its content into
 * an image formatted file, or to read one
 */
class Image {
    public:
    	/**
    	 * \brief Constructor
    	 * \param x : width resolution
    	 * \param y : height resolution
    	 */
		Image(int x, int y) :
			_res_x(x), _res_y(y)
		{
			_matrix.resize(x);
			for (int i = 0; i < x; i++)
				_matrix.at(i).resize(y);
		}
		Image(std::string);

		int get_res_x() const { return _res_x ; } ///< Returns the width resolution
		int get_res_y() const { return _res_y ; } ///< Returns the height resolution

        void add_color(boost::shared_ptr< Color >, int, int);	///< Add a new pixel to the matrix

        boost::shared_ptr<Color> get_color(int x, int y) const ///< Returns the color corresponding to the given coordinates
        {
            return _matrix[x][y];
        }
        static void special_byte(int, int *, int *, int *, int *);	///< Byte special conversion (used when saving into BMP/TGA)
        void save_to_TGA(const std::string&) const; ///< Saving in a TGA file

    private:
        int _res_x, _res_y;		///< Width and Heigth of image
        std::vector< std::vector< boost::shared_ptr< Color > > > _matrix;	///< Matrix of pixels (Color)
};

#endif
