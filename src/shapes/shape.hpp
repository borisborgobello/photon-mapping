#ifndef SHAPE_HPP_
#define SHAPE_HPP_

/**
 * \file shape.hpp
 * \brief Declaration of base Shape class
 * \author B.BORGOBELLO / T.FEIGLER
 */

#include <boost/smart_ptr/shared_ptr.hpp>
#include "textures/texture.hpp"
#include "launchables/photon.hpp"
#include "launchables/ray.hpp"

/**
 * \class Shape
 * \brief Base class representing an object
 *
 * It can be either a surface of a volume.
 */
class Shape
{
public:
    /**
	 * \brief Constructor
	 *
	 * Constructor of class Shape
	 *
	 * \param absorp : absorption probability
	 * \param reflect : reflection probability
	 * \param tex : pointer to the texture of this Shape
	 */
	Shape(double absorp, double reflect, Texture* tex) :
		_absorption_prob(absorp), _reflection_prob(reflect),
		_texture( boost::shared_ptr< Texture >(tex) ) {}

	boost::shared_ptr<Texture> get_texture() const { return _texture ; } ///< Returns a smart pointer to the texture used by this shape
	double get_absorption_prob() const { return _absorption_prob ; } ///< Returns the absorption probability of this shape
	double get_reflection_prob() const { return _reflection_prob ; } ///< Returns the reflection probability of this shape

	virtual bool is_intersected_by(const Launchable&) const = 0 ; ///< Returns whether this Shape is intersected by a given launchable
	virtual Couple3D get_nearest_intersection_with_normal( const Launchable& ) const = 0 ; ///< Returns the nearest intersection (point/normal) of the given launchable with this shape
	virtual bool redirect_photon( const Couple3D&, Photon& ) const = 0 ; ///< Redirects (or not) a given photon depending on the probilities of this shape
	virtual std::pair<Ray, Ray> divide_ray( const Couple3D&, const Ray& ) const = 0 ; ///< Returns the division (reflected/refracted) of an incoming ray at the couple position
	virtual Color get_color_at(const Point3D&) const = 0; ///< Returns the color at this point of the shape

protected:
	double _absorption_prob; ///< Absorption probabilities
	double _reflection_prob ; ///< Reflection probabilities
	boost::shared_ptr<Texture> _texture ; ///< A smart pointer to the texture used by this Shape
};

#endif /* SHAPE_HPP_ */
