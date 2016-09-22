#ifndef PROCEDURAL_HPP_
#define PROCEDURAL_HPP_

/**
 * \file procedural.hpp
 * \brief Declaration of Procedural class
 * \author T.FEIGLER
 */

#include "texture.hpp"
#include <geometry.hpp>

/**
 * \class Procedural
 * \brief Derived from Texture, representing a pattern
 */
class Procedural : public Texture
{
public:
/**
 * \brief Defines the texture's orientation parameters
 * \param text_x : a first orientation vector for the texture
 * \param text_y : a second orientation vector for the texture
 */
    Procedural(Vector3D text_x, Vector3D text_y)
    {
        _text_x = (text_x.norm() == 0) ? text_x : text_x.normalized();
        _text_y = (text_y.norm() == 0) ? text_y : text_y.normalized();
    }
	virtual Color operator()(double, double) const = 0 ; ///< Used to determine the color
	Color get_color(double x, double y) const ///< Returns the color matching the coordinates
	{
		return operator()(x, y) ;
	}
    Vector3D get_text_x() { return _text_x; } ///< Returns the first orientation vector
    Vector3D get_text_y() { return _text_y; } ///< Returns the second orientation vector
    void set_text_x(Vector3D& text_x) { _text_x = text_x;} ///< Sets the first orientation vector
    void set_text_y(Vector3D& text_y) { _text_y = text_y;} ///< Sets the second orientation vector
private:
    Vector3D _text_x, _text_y; ///< Orientation vectors for textures
} ;

#endif
