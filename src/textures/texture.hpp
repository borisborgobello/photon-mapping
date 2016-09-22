#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

/**
 * \file texture.hpp
 * \brief Declaration of Texture class
 * \author T.FEIGLER
 */

#include "color.hpp"


/**
 * \class Texture
 * \brief Base class representing any texture
 */
class Texture
{
public:
	Texture() {} ///< Constructor
	virtual Color get_color(double, double) const = 0 ; ///< Returns the Color correponding to the given coordinates
};

#endif
