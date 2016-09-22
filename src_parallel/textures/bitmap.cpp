
/**
 * \file bitmap.cpp
 * \brief Implementation of class Bitmap
 * \author BORGOBELLO Boris / FEIGLER Thomas
 */

#include "bitmap.hpp"

/**
 * \brief Returns the color corresponding to
 * the given coordinates
 *
 * If x and y are bigger than the size of the image,
 * the pattern is periodic and will be repeated infinitly
 *
 * \param x, y : Coordinates for the texture
 */
Color Bitmap::operator()(double x, double y) const
{
	double square_pos_x, square_pos_y ;

	square_pos_x = (x - ((int)(x / _x_dim))*_x_dim)/_x_dim ;
	square_pos_y = (y - ((int)(y / _y_dim))*_y_dim)/_y_dim ;

	int pixel_x = std::abs(square_pos_x*_image.get_res_x());
	int pixel_y = _image.get_res_y() - std::abs(square_pos_y*_image.get_res_y());

    return *(_image.get_color(pixel_x, pixel_y).get());
}
