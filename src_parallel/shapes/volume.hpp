#ifndef VOLUME_HPP_
#define VOLUME_HPP_

/**
 * \file volume.hpp
 * \brief Declaration of Volume class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include "shape.hpp"
#include "geometry.hpp"

/**
 * \class Volume
 * \brief Derived from Shape, representing a volume
 *
 * Considered not empty, the volume can reflect and also
 * refract changing the direction of the incoming Launchable
 * from inside and from outside
 * It can be intersected by a Launchable
 */
class Volume : public Shape
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Volume
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param refract : refraction probability
	 * \param index : index (material) of this volume
	 * \param tex : pointer to the texture of this volume
	 */
	Volume(double absorp, double reflect, double refract, double index, Texture* tex) :
		Shape(absorp, reflect, tex), _refraction_prob(refract), _ref_index(index) {}

	double get_refraction_prob() const { return _refraction_prob ; } ///< Returns the refraction probability

	virtual Couple3D get_random_point_and_normal() const = 0 ; ///< Returns a random surface point and a random normal

protected:
	double _refraction_prob ; ///< The refraction probability
	double _ref_index ; ///< The index (material) of the volume
};

#endif /* VOLUME_HPP_ */
