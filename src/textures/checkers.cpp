/**
 * \file checkers.cpp
 * \brief Implementation of Checkers
 * \author T.FEIGLER / B. BORGOBELLO
 */

#include "checkers.hpp"

/**
 * \brief Returns the head or tail color
 *
 * Determines whether the given position is
 *
 * \param x, y : Coordinates for the texture
 */
Color Checkers::operator()(double x, double y) const
{
	int square_pos_x, square_pos_y ;

	square_pos_x = x / _x_dim ;
	square_pos_y = y / _y_dim ;

    if (x*y >= 0) {
        if( (square_pos_x + square_pos_y) % 2 == 0)
            return _color_even ;
        else
            return _color_odd ;
    }
    else {
        if( (square_pos_x + square_pos_y) % 2 == 0)
            return _color_odd ;
        else
            return _color_even ;
    }
}
