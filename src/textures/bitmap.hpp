
#ifndef BITMAP_HPP_
#define BITMAP_HPP_

/**
 * \file bitmap.hpp
 * \brief Declaration of Bitmap class
 * \author B. BORGOBELLO
 */

#include "procedural.hpp"
#include <image.hpp>

/**
 * \class Bitmap
 * \brief Derived from Procedural, representing an Imaged texture
 */
class Bitmap : public Procedural
{
public:
    /**
	 * \brief Constructor of Bitmap
	 * \param text_x : a first orientation vector for the texture
	 * \param text_y : a second orientation vector for the texture
	 * \param x_dim : dimension x
	 * \param y_dim : dimension y
	 * \param file : name of file .TGA to import
	 */
	Bitmap(Vector3D text_x, Vector3D text_y, double x_dim, double y_dim, std::string file) :
		Procedural(text_x, text_y), _x_dim(x_dim), _y_dim(y_dim), _image(file) {}

	Color operator()(double, double) const; ///< Returns the color matching the coordinates

private:
	double _x_dim, _y_dim ; ///< Dimensions
	Image _image; ///< Image of the file
};

#endif
