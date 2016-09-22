#ifndef CHECKERS_HPP_
#define CHECKERS_HPP_

/**
 * \file checkers.hpp
 * \brief Declaration of Checkers class
 * \author T.FEIGLER
 */

#include "procedural.hpp"


/**
 * \class Checkers
 * \brief Derived from Procedural, representing a checkers pattern
 */
class Checkers : public Procedural
{
public:
    /**
	 * \brief Constructor of Checkers
	 * \param text_x : a first orientation vector for the texture
	 * \param text_y : a second orientation vector for the texture
	 * \param x_dim : dimension x of a square
	 * \param y_dim : dimension y of a square
	 * \param even : first color of the checkers
	 * \param odd : second color of the checkers
	 */
	Checkers(Vector3D text_x, Vector3D text_y, double x_dim, double y_dim, Color even, Color odd) :
		Procedural(text_x, text_y), _x_dim(x_dim), _y_dim(y_dim),
		_color_even(even), _color_odd(odd) {}

	Color operator()(double, double) const; ///< Returns the color corresponding matching the coordinates

private:
	double _x_dim, _y_dim ; ///< Dimensions
	Color _color_even, _color_odd ; /// Colors
};

#endif /* CHECKERS_HPP_ */
