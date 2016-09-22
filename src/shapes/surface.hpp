#ifndef SURFACE_HPP_
#define SURFACE_HPP_

/**
 * \file surface.hpp
 * \brief Declaration of Surface class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "shape.hpp"

/**
 * \class Surface
 * \brief Derived from Shape, representing a surface
 *
 * Considered infinitly thin, the surface can reflect,
 * and refract by letting a Launchable pass through without
 * any change to its direction
 * It can be intersected by a Launchable
 */
class Surface : public Shape
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Surface
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param transp : transparency probability
	 * \param tex : pointer to the texture of this surface
	 */
	Surface(double absorp, double reflect, double transp, Texture* tex) :
		Shape(absorp, reflect, tex), _transparency_prob(transp) {}

	double get_transparency_prob() const { return _transparency_prob ; } ///< Returns the transparency probability

protected:
	double _transparency_prob ; ///< The transparency probability
};

#endif /* SURFACE_HPP_ */
