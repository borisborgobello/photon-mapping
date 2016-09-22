#ifndef COLORED_HPP_
#define COLORED_HPP_

/**
 * \file colored.hpp
 * \brief Declaration of Colored class
 * \author T.FEIGLER
 */

#include "textures/texture.hpp"

/**
 * \class Colored
 * \brief Derived from Texture, representing a colored texture
 */
class Colored : public Texture
{
public:
    /**
	 * \brief Constructor of Colored
	 * \param c : color of the texture
	 */
	Colored(Color c) : _color(c) {}

	Color get_color(double, double) const { return _color ; } ///< Returns the color of this texture

private:
	Color _color ; ///< The color of this texture
};

#endif /* COLORED_HPP_ */
